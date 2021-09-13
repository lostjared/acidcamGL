mkdir -p dmg_build/acidcamGL/acidcamGL/

cp -rfv filters dmg_build/acidcamGL/acidcamGL/
cp -rfv autofilter dmg_build/acidcamGL/acidcamGL/
cp -rfv playlist dmg_build/acidcamGL/acidcamGL/
#cp -rfv shortcut dmg_buildshortcut/acidcamGL/acidcamGL/shortcut
cp -rfv custom dmg_build/acidcamGL/acidcamGL/
cp -rfv /Applications/acidcamGL/acidcamGL.app dmg_build/acidcamGL/acidcamGL/
hdiutil create -volname acidcamGL -srcfolder dmg_build/acidcamGL -ov -format UDZO acidcamGL.dmg
