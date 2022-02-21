#!/bin/bash
echo "Try to update po file from cpp code"
_language='fr'
echo
cd ../po/$_language/
echo "################### Update french.po ###################"
echo '' > messages.po
find ../../src -type f -iname "*.cpp" | xgettext -j --from-code=UTF-8 -f -
msgmerge -N $_language.po messages.po > new.po
mv new.po $_language.po
rm messages.po
echo
echo "################### Create kingsandships.mo ###################"
msgfmt -c -v -o kingsandships.mo fr.po
echo
echo "################### Install kingsandships.mo for quick testing ###################"
sudo cp kingsandships.mo /usr/share/locale/fr/LC_MESSAGES/
ls -lh /usr/share/locale/fr/LC_MESSAGES/kingsandships.mo
