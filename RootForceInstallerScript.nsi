;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Root Force"
  OutFile "RootForceSetup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\ZombieShroom\Root Force"
  
  ;Get installation folder from registry if available
  ;InstallDirRegKey HKCU "Software\Root Force" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections


Section "Root Force" SecRootForce

  SetShellVarContext all 
  SetOutPath "$INSTDIR"
  SectionIn RO

  File "Release\RootForce.exe"
  File "Release\*.dll"
  File /r "Release\platforms\*.*"
  File /r "Release\imageformats\*.*"
  File "Release\awesomium_process.exe"
  File /r "Release\Assets\*.*"
  File "Release\README.txt"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\RootForce.lnk" "$INSTDIR\RootForce.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd


Section "Treenity" SecTreenity

  SetShellVarContext all 
  SetOutPath "$INSTDIR"

  File "Release\Treenity.exe"
  ;File "Release\ParticleEditor.exe"
  File /r "Release\Resources\*.*"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Treenity.lnk" "$INSTDIR\Treenity.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\ParticleEditor.lnk" "$INSTDIR\ParticleEditor.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecRootForce ${LANG_ENGLISH} "Installs the complete game."
  LangString DESC_SecTreenity ${LANG_ENGLISH} "Installs the level and particle editor."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRootForce} $(DESC_SecRootForce)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecTreenity} $(DESC_SecTreenity)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  SetShellVarContext all
  Delete "$DESKTOP\Root Force.lnk"
  RMDir /r "$SMPROGRAMS\Root Force\"
  RMDir /r "$INSTDIR"
  Delete $INSTDIR\Uninstall.exe

SectionEnd