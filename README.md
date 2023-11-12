![Build pass](https://github.com/marqdevx/mm-cs2-coach/actions/workflows/compile-metamod-addon.yaml/badge.svg)

## IMPORTANT
This repository is based on https://github.com/Source2ZE/CS2Fixes/
Removing "unnecessary" features from the original addon.
Thanks for the work!

```
DISCLAIMER: *not* responsible for any issues or breakages caused by the plugin. 
```

### Fix server crashes
In case you have non-expected server crashes, it might be due the `addons\cs2scrim\gamedata\cs2fixes.games.txt` is not up-to-date.
I might not update that text until a next "official" release of the addon, so in order to fix that file:
The original addon repository it's actively updated, so copy the content of this https://github.com/Source2ZE/CS2Fixes/blob/main/gamedata/cs2fixes.games.txt onto your `addons\cs2scrim\gamedata\cs2fixes.games.txt`

# Addon information
This metamod addon ONLY adds the workaround to set a player as a coach on competitive matches.

## Available Commands

  `.coach`: set to coach  
  `.uncoach`: stop coaching, back as a player  
      aliases: `.nocoach`, `.coachstop`, `.coachend`

## Compilation

### Requirements

- [Metamod:Source](https://www.sourcemm.net/downloads.php/?branch=master) (build 1219 or higher)
- [AMBuild](https://wiki.alliedmods.net/Ambuild)

### Instructions
[![Set up video](https://img.youtube.com/vi/thk78MDsQnc/0.jpg)]([https://www.youtube.com/watch?v=YOUTUBE_VIDEO_ID_HERE](https://www.youtube.com/watch?v=thk78MDsQnc))  
Set up video: https://www.youtube.com/watch?v=thk78MDsQnc

## Authors from the [original repository](https://github.com/Source2ZE/CS2Fixes/)
- [@xen-000](https://github.com/xen-000)
- [@poggicek](https://github.com/poggicek)

## Credits
- [CKRAS Hosting Team](https://www.ckras.com/en) They liked the cs2scrim addon, they contacted me and helped trying out the best way to make this work, find bugs and give feedback.
