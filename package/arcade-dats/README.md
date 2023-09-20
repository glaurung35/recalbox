Install `7z` and `xsltproc`:
```bash
apt-get install 7zip xsltproc
```

Update files for a system:
```bash
python3 updater.py -target <corename>
```

Example:
```bash
python3 updater.py -target fbneo
```

Available targets:
- advancemame
- mame2000
- mame2003
- mame2003+
- mame2010
- mame2015
- mame
- pifba
- fbneo
- supermodel
- flycast