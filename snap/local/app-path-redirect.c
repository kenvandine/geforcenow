/*
 * NVIDIA's GeForceNOW binary looks for its shared/writable config (e.g.
 * mall/shared/assets/config/config.json) at /app/... when it detects it's
 * running inside a flatpak sandbox (flatpak always mounts the app's install
 * tree at /app), and otherwise falls back to
 * "$HOME/.var/NVIDIA/GeForceNOW/<same relative path>" -- a location that is
 * never populated outside of a real flatpak/native-installer environment.
 * Under strict snap confinement neither path exists: there's no /app (and
 * snapd's `layout` can't create a new top-level /app on core24), and $HOME
 * is the per-revision snap data dir, which never gets that .var tree seeded
 * into it either.
 *
 * This LD_PRELOAD shim intercepts the libc file-access calls and rewrites
 * both hardcoded forms back to "$SNAP/<same relative path>", so those
 * lookups resolve into the snap's installed tree instead of failing.
 *
 * It also intercepts the exec family: GeForceNOW launches its helper
 * "container" process (cef/GeForceNOWContainer, which hosts the mandatory
 * GfnBackgroundAgent/GameStreamClientAgent plugins from nvc/gfn/*.so) via
 * the same hardcoded /app/... path. Without this, that exec silently fails,
 * the container never starts, and the app eventually shows a "Problem
 * Detected" dialog once the mandatory-plugin load times out.
 *
 * Finally, it intercepts SDL_CreateWindow: GeForceNOW creates its main
 * window with SDL_WINDOW_BORDERLESS. Under XWayland this didn't matter --
 * mutter's X11 window-manager path decorates top-level windows regardless
 * of what the client requests -- but under native Wayland (forced in
 * geforcenow-launch to fix HiDPI/multi-monitor scaling; see there) a
 * borderless client genuinely gets no titlebar at all, since decoration is
 * a pure client opt-in over Wayland. Force the window bordered right after
 * creation so libdecor draws a titlebar again (move/close).
 */

#define _GNU_SOURCE
#include <dirent.h>
#include <dlfcn.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *snap_prefix(void) {
    static const char *snap = NULL;
    static int looked_up = 0;
    if (!looked_up) {
        snap = getenv("SNAP");
        looked_up = 1;
    }
    return snap;
}

static char *rewrite_with_suffix(const char *suffix) {
    /* suffix must not start with '/': result is "$SNAP/<suffix>". */
    const char *snap = snap_prefix();
    size_t snap_len, suffix_len;
    char *out;

    if (!snap || !*snap)
        return NULL;

    snap_len = strlen(snap);
    suffix_len = strlen(suffix);
    out = malloc(snap_len + 1 + suffix_len + 1);
    if (!out)
        return NULL;

    memcpy(out, snap, snap_len);
    out[snap_len] = '/';
    memcpy(out + snap_len + 1, suffix, suffix_len + 1);
    return out;
}

/* Returns a malloc'd rewritten path if `path` matches one of GeForceNOW's
 * two hardcoded install-tree conventions, otherwise NULL (caller should use
 * the original path). */
static char *redirect(const char *path) {
    static const char marker[] = "/.var/NVIDIA/GeForceNOW/";
    const char *hit;

    if (!path)
        return NULL;

    if (strncmp(path, "/app/", 5) == 0)
        return rewrite_with_suffix(path + 5);

    hit = strstr(path, marker);
    if (hit)
        return rewrite_with_suffix(hit + sizeof(marker) - 1);

    return NULL;
}

#define REAL(name) \
    static typeof(name) *real_##name = NULL; \
    if (!real_##name) \
        real_##name = dlsym(RTLD_NEXT, #name);

int open(const char *path, int flags, ...) {
    va_list ap;
    mode_t mode = 0;
    char *redirected;
    int rc;
    REAL(open)

    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    redirected = redirect(path);
    rc = real_open(redirected ? redirected : path, flags, mode);
    free(redirected);
    return rc;
}

int open64(const char *path, int flags, ...) {
    va_list ap;
    mode_t mode = 0;
    char *redirected;
    int rc;
    REAL(open64)

    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    redirected = redirect(path);
    rc = real_open64(redirected ? redirected : path, flags, mode);
    free(redirected);
    return rc;
}

int openat(int dirfd, const char *path, int flags, ...) {
    va_list ap;
    mode_t mode = 0;
    char *redirected;
    int rc;
    REAL(openat)

    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    redirected = redirect(path);
    rc = real_openat(dirfd, redirected ? redirected : path, flags, mode);
    free(redirected);
    return rc;
}

FILE *fopen(const char *path, const char *mode) {
    char *redirected = redirect(path);
    FILE *fp;
    REAL(fopen)
    fp = real_fopen(redirected ? redirected : path, mode);
    free(redirected);
    return fp;
}

FILE *fopen64(const char *path, const char *mode) {
    char *redirected = redirect(path);
    FILE *fp;
    REAL(fopen64)
    fp = real_fopen64(redirected ? redirected : path, mode);
    free(redirected);
    return fp;
}

int access(const char *path, int mode) {
    char *redirected = redirect(path);
    int rc;
    REAL(access)
    rc = real_access(redirected ? redirected : path, mode);
    free(redirected);
    return rc;
}

int stat(const char *path, struct stat *buf) {
    char *redirected = redirect(path);
    int rc;
    REAL(stat)
    rc = real_stat(redirected ? redirected : path, buf);
    free(redirected);
    return rc;
}

int lstat(const char *path, struct stat *buf) {
    char *redirected = redirect(path);
    int rc;
    REAL(lstat)
    rc = real_lstat(redirected ? redirected : path, buf);
    free(redirected);
    return rc;
}

int stat64(const char *path, struct stat64 *buf) {
    char *redirected = redirect(path);
    int rc;
    REAL(stat64)
    rc = real_stat64(redirected ? redirected : path, buf);
    free(redirected);
    return rc;
}

int lstat64(const char *path, struct stat64 *buf) {
    char *redirected = redirect(path);
    int rc;
    REAL(lstat64)
    rc = real_lstat64(redirected ? redirected : path, buf);
    free(redirected);
    return rc;
}

int execve(const char *path, char *const argv[], char *const envp[]) {
    char *redirected = redirect(path);
    int rc;
    REAL(execve)
    rc = real_execve(redirected ? redirected : path, argv, envp);
    free(redirected); /* only reached if execve fails */
    return rc;
}

int execv(const char *path, char *const argv[]) {
    char *redirected = redirect(path);
    int rc;
    REAL(execv)
    rc = real_execv(redirected ? redirected : path, argv);
    free(redirected);
    return rc;
}

int execvp(const char *file, char *const argv[]) {
    char *redirected = redirect(file);
    int rc;
    REAL(execvp)
    rc = real_execvp(redirected ? redirected : file, argv);
    free(redirected);
    return rc;
}

void *dlopen(const char *filename, int flags) {
    char *redirected = redirect(filename);
    void *handle;
    REAL(dlopen)
    handle = real_dlopen(redirected ? redirected : filename, flags);
    free(redirected);
    return handle;
}

DIR *opendir(const char *path) {
    char *redirected = redirect(path);
    DIR *dp;
    REAL(opendir)
    dp = real_opendir(redirected ? redirected : path);
    free(redirected);
    return dp;
}

int posix_spawn(pid_t *pid, const char *path,
                 const posix_spawn_file_actions_t *file_actions,
                 const posix_spawnattr_t *attrp, char *const argv[],
                 char *const envp[]) {
    char *redirected = redirect(path);
    int rc;
    REAL(posix_spawn)
    rc = real_posix_spawn(pid, redirected ? redirected : path, file_actions,
                           attrp, argv, envp);
    free(redirected);
    return rc;
}

typedef void SDL_Window;

/* GeForceNOW doesn't actually request a borderless window (confirmed: its
 * SDL_CreateWindow flags never include SDL_WINDOW_BORDERLESS) -- the real
 * reason it had no titlebar under native Wayland was that NVIDIA's bundled
 * libSDL2 was compiled without libdecor support at all (see the sdl2 part
 * in snapcraft.yaml, which replaces it with one that has). These hooks are
 * kept as a harmless belt-and-suspenders guard in case that ever changes. */
void SDL_SetWindowBordered(SDL_Window *window, int bordered) {
    REAL(SDL_SetWindowBordered)
    real_SDL_SetWindowBordered(window, 1);
}

SDL_Window *SDL_CreateWindow(const char *title, int x, int y, int w, int h,
                              unsigned int flags) {
    SDL_Window *win;
    REAL(SDL_CreateWindow)

    win = real_SDL_CreateWindow(title, x, y, w, h, flags);
    if (win)
        SDL_SetWindowBordered(win, 1);
    return win;
}
