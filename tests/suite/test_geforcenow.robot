*** Settings ***
Documentation    Test cases for geforcenow snap
Resource         kvm.resource


*** Test Cases ***
Geforcenow Launches And Renders
    [Documentation]    Verify geforcenow snap launches and renders a UI on Mir
    [Tags]    smoke    yarf:certification_status: blocker
    Log Screenshot
