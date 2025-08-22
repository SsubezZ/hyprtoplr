# hyprtoplr

Top Layer visibility plugin for Hyprland. Adds some top layer visibility dispatchers to Hyprland.
[mainly with fullscreen]

https://github.com/user-attachments/assets/5c054224-4a0a-459e-9fb1-fcb9b82fdaf5

## Installation

### Through hyprpm

```
hyprpm add https://github.com/SsubezZ/hyprtoplr
hyprpm enable hyprtoplr
hyprpm reload
```

### Manual

```
git clone https://github.com/SsubezZ/hyprtoplr
cd hyprtoplr
make all

hyprctl plugin load /path/to/hyprtoplr.so # should be absoulute
```

## Dispatchers

All dispatchers here are called `plugin:hytop:<name>` e.g. `plugin:hytop:toggle`.

| Name           | Description                                                                                      | Usage                                      |
| -------------- | ------------------------------------------------------------------------------------------------ | ------------------------------------------ |
| **show**       | Forces top layers to be visible, regardless of workspace fullscreen state.                       | `hyprctl dispatch plugin:hytay:show`       |
| **hide**       | Forces top layers to be hidden, regardless of workspace fullscreen state.                        | `hyprctl dispatch plugin:hytay:hide`       |
| **toggle**     | Toggles top layers visibility, but respects fullscreen rules of the active or special workspace. | `hyprctl dispatch plugin:hytay:toggle`     |
| **justtoggle** | Toggles top layers visibility unconditionally, ignoring workspace and fullscreen rules.          | `hyprctl dispatch plugin:hytay:justtoggle` |

## Usage

Toggle with swaync to avoid awkward empty area on the top

```
bind = WIN, Z, plugin:hytop:toggle
bind = WIN, Z, exec, swaync-client -t
```

| Before                                                                                                                                | After                                                                                                                               |
| ------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| <img width="1920" height="1080" alt="before" src="https://github.com/user-attachments/assets/ac3bd8bb-d2c3-4c41-8f4e-2d3bc2d2ad98" /> | <img width="1920" height="1080" alt="after" src="https://github.com/user-attachments/assets/d41c26e9-2164-40a7-b43d-76ba39a1be7a"/> |
|                                                                                                                                       |

Close swaync and hide the layer

```
bind = WIN SHIFT, Z, exec, swaync-client -cp
bind = WIN SHIFT, Z, plugin:hytop:hide
```
