// All of the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.
const { ipcRenderer } = require('electron');

Object.defineProperty(navigator, 'webdriver', {
  get: () => undefined,
});

if (window.process && window.process.versions) {
    delete window.process.versions['electron'];
}

window.addEventListener("DOMContentLoaded", () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector);
    if (element) element.innerText = text;
  };

  for (const type of ["chrome", "node", "electron"]) {
    replaceText(`${type}-version`, process.versions[type]);
  }
});

(function mockChromeUserAgent() {
  let oiginalVoices = window.speechSynthesis.getVoices();
  window.speechSynthesis.getVoices = function () {
    return [
      {
        voiceURI: "Google US English",
        name: "Google US English",
        lang: "en-US",
        localService: false,
        default: false,
      },
    ];
  };

  //wait some arbitraty time before cleaning up the mess we did previously
  setTimeout(() => {
    window.speechSynthesis.getVoices = function () {
      return oiginalVoices;
    };
  }, 10_000);
})();

// List for DOMContentLoaded event
window.addEventListener('DOMContentLoaded', () => {
    // Listen for click events and fire open-external-link as needed
    document.addEventListener('click', (event) => {
        if (event.target.tagName === 'A' && event.target.href.startsWith('http')) {
            event.preventDefault();
            ipcRenderer.send('open-external-link', event.target.href);
        }
    });
});
