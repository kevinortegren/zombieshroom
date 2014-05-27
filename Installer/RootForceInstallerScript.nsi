;--------------------------------
;Includes

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Root Force"
  OutFile "RootForceSetup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\ZombieShroom\RootForce"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Root Force" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_WELCOMEFINISHPAGE_BITMAP "InstallerImageLarge.bmp" 
  !define MUI_HEADERIMAGE 
  !define MUI_HEADERIMAGE_BITMAP "InstallerImageSmall.bmp"
  !define MUI_ICON "RootForceIco.ico" 

;--------------------------------
;Pages

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\RootForce"
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
  SectionIn RO

  ;Add the files to be extracted (and specify where)
  SetOutPath "$INSTDIR"
  File "Release\RootForce.exe"
  File "Release\Treenity.exe"
  File "Release\*.dll"
  File "Release\awesomium_process.exe"
  File "Release\README.txt"
  File "Release\vcredist_x86.exe"
  
  SetOutPath "$INSTDIR\platforms\"
  File /r "Release\platforms\*.*"
  
  SetOutPath "$INSTDIR\imageformats\"
  File /r "Release\imageformats\*.*"
  
  SetOutPath "$INSTDIR\Assets\"
  File /r "Release\Assets\*.*"

  ;Create uninstaller
  SetOutPath "$INSTDIR"
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\RootForce.lnk" "$INSTDIR\RootForce.exe"
	CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Treenity.lnk" "$INSTDIR\Treenity.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
  ; Install the Visual Studio 11 redistributable.
  ReadRegDWORD $1 HKLM "SOFTWARE\Microsoft\VisualStudio\11.0_Config\VC\Runtimes\x86" "Installed"
  IntCmp $1 1 redist_already_installed
  ExecWait "$INSTDIR/vcredist_x86.exe"
  redist_already_installed:
  
  ;Store installation folder
  WriteRegStr HKCU "Software\ZombieShroom\RootForce" "" $INSTDIR

SectionEnd


Section "Treenity" SecTreenity
	SectionIn RO
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
  SetOutPath "$TEMP"
  
  MessageBox MB_OK "The install directory is: $INSTDIR"
  ;Delete "$INSTDIR\*.*"
  ;Delete "$INSTDIR\Uninstall.exe"
  RMDir /r "$INSTDIR"
  
  DeleteRegKey /ifempty HKCU "Software\ZombieShroom\RootForce"
SectionEnd