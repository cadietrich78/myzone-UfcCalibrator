/*
* COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
*
*
* For the purposes of this copyright and license, "Contributing Authors"
* is defined as the following set of individuals:
*
*    Carlos Augusto Dietrich (cadietrich@gmail.com)
*
* This library is supplied "AS IS".  The Contributing Authors disclaim
* all warranties, expressed or implied, including, without limitation,
* the warranties of merchantability and of fitness for any purpose.
* The Contributing Authors assume no liability for direct, indirect,
* incidental, special, exemplary, or consequential damages, which may
* result from the use of the this library, even if advised of the
* possibility of such damage.
*
* Permission is hereby granted to use, copy, modify, and distribute this
* source code, or portions hereof, for any purpose, without fee, subject
* to the following restrictions:
*
* 1. The origin of this source code must not be misrepresented.
*
* 2. Altered versions must be plainly marked as such and must not be
*    misrepresented as being the original source.
*
* 3. This Copyright notice may not be removed or altered from any source
*    or altered source distribution.
*
* The Contributing Authors specifically permit, without fee, and
* encourage the use of this source code as a component in commercial
* products. If you use this source code in a product, acknowledgment
* is not required but would be appreciated.
*
*
* "Software is a process, it's never finished, it's always evolving.
* That's its nature. We know our software sucks. But it's shipping!
* Next time we'll do better, but even then it will be shitty.
* The only software that's perfect is one you're dreaming about.
* Real software crashes, loses data, is hard to learn and hard to use.
* But it's a process. We'll make it less shitty. Just watch!"
*/

#if !defined(STRING_RESOURCE_INCLUDED)
#define STRING_RESOURCE_INCLUDED

// TODO: (06-Mar-2015) A facade for the application string resource manager. In the future, all strings will be stored somewhere else (a JSON file on the web?), and these macros will be converted to calls for these strings.

#define UFC_STRING_RESOURCE_0001 "Cyclops Calibration Tool"
#define UFC_STRING_RESOURCE_0002 "Calibration quality"
#define UFC_STRING_RESOURCE_0003 "Expert mode"
#define UFC_STRING_RESOURCE_0004 "Cyclops calibration..."
#define UFC_STRING_RESOURCE_0005 "Exit"
#define UFC_STRING_RESOURCE_0006 "Exit the application"
#define UFC_STRING_RESOURCE_0007 "Show error messages"
#define UFC_STRING_RESOURCE_0008 "Show error messages."
#define UFC_STRING_RESOURCE_0009 "File"
#define UFC_STRING_RESOURCE_0010 "Help"
#define UFC_STRING_RESOURCE_0011 "Debugging"
#define UFC_STRING_RESOURCE_0012 "Export"
#define UFC_STRING_RESOURCE_0013 "New dataset"
#define UFC_STRING_RESOURCE_0014 "Do you want to replace the current frame? If not, a new play will be created."
#define UFC_STRING_RESOURCE_0015 "Export file"
#define UFC_STRING_RESOURCE_0016 "JSON file (*.json)"
#define UFC_STRING_RESOURCE_0017 "Marker"
#define UFC_STRING_RESOURCE_0018 "Size"
#define UFC_STRING_RESOURCE_0019 "Zoom"
#define UFC_STRING_RESOURCE_0020 "Export Cyclops calibration"
#define UFC_STRING_RESOURCE_0021 "Camera"
#define UFC_STRING_RESOURCE_0022 "calibration.pinholeCamera"
#define UFC_STRING_RESOURCE_0023 "Optimization settings"
#define UFC_STRING_RESOURCE_0024 "Camera settings"
#define UFC_STRING_RESOURCE_0025 "Please inform the camera index:"
#define UFC_STRING_RESOURCE_0026 "Camera settings exported successfully!"
#define UFC_STRING_RESOURCE_0027 "Failed to export camera settings!"
#define UFC_STRING_RESOURCE_0028 "Octagon size"
#define UFC_STRING_RESOURCE_0029 "All markers"
#define UFC_STRING_RESOURCE_0030 "Clear"
#define UFC_STRING_RESOURCE_0031 "JSON file (*.json)"
#define UFC_STRING_RESOURCE_0032 "Pinhole camera file (*.pinholeCamera)"
#define UFC_STRING_RESOURCE_0061 "All Supported Formats (*.png; *.jpg)"
#define UFC_STRING_RESOURCE_0087 "Edit"
#define UFC_STRING_RESOURCE_0089 "PNG file (*.png)"
#define UFC_STRING_RESOURCE_0110 "Failed to get a RGBA buffer. The rendering context results will be suboptimal."
#define UFC_STRING_RESOURCE_0111 "Failed to get a double buffer. The rendering context results will be suboptimal."
#define UFC_STRING_RESOURCE_0112 "Failed to get a depth buffer. The rendering context results will be suboptimal."
#define UFC_STRING_RESOURCE_0113 "Failed to get sample buffers. The rendering context results will be suboptimal."
#define UFC_STRING_RESOURCE_0114 "Make it better!"
#define UFC_STRING_RESOURCE_0129 "Cyclops"
#define UFC_STRING_RESOURCE_0130 "Cyclops Calibration Tool"
#define UFC_STRING_RESOURCE_0131 "0.1"
#define UFC_STRING_RESOURCE_0133 "Ok"
#define UFC_STRING_RESOURCE_0134 "Cancel"
#define UFC_STRING_RESOURCE_0135 "Settings"
#define UFC_STRING_RESOURCE_0138 "Settings..."
#define UFC_STRING_RESOURCE_0141 "Strike zone view"
#define UFC_STRING_RESOURCE_0142 "Text size"
#define UFC_STRING_RESOURCE_0148 "Opening play and preloading game data (this might take a few seconds)..."
#define UFC_STRING_RESOURCE_0149 "Done"
#define UFC_STRING_RESOURCE_0152 "Windows"
#define UFC_STRING_RESOURCE_0163 "Speed"
#define UFC_STRING_RESOURCE_0164 "Fast"
#define UFC_STRING_RESOURCE_0165 "Slow"
#define UFC_STRING_RESOURCE_0176 "Warning"
#define UFC_STRING_RESOURCE_0193 "Layout line thickness"
#define UFC_STRING_RESOURCE_0204 "Information text size"
#define UFC_STRING_RESOURCE_0209 "Small"
#define UFC_STRING_RESOURCE_0210 "Medium"
#define UFC_STRING_RESOURCE_0211 "Large"
#define UFC_STRING_RESOURCE_0212 "Low"
#define UFC_STRING_RESOURCE_0213 "Medium"
#define UFC_STRING_RESOURCE_0214 "High"
#define UFC_STRING_RESOURCE_0243 "(no game selected)"
#define UFC_STRING_RESOURCE_0244 "Game"
#define UFC_STRING_RESOURCE_0251 "Ready (press F1 for help)"
#define UFC_STRING_RESOURCE_0253 "Cheat sheet"
#define UFC_STRING_RESOURCE_0254 "Shows a list of the most common actions and commands of the tool"
#define UFC_STRING_RESOURCE_0289 "Number of iterations"
#define UFC_STRING_RESOURCE_0290 "Population size"
#define UFC_STRING_RESOURCE_0291 "Selected individuals"
#define UFC_STRING_RESOURCE_0292 "Maintaining diversity"
#define UFC_STRING_RESOURCE_0293 "Mutation threshold"
#define UFC_STRING_RESOURCE_0294 "Optical center step"
#define UFC_STRING_RESOURCE_0295 "Field of view step"
#define UFC_STRING_RESOURCE_0296 "Yaw (pan) step"
#define UFC_STRING_RESOURCE_0297 "Pitch (tilt) step"
#define UFC_STRING_RESOURCE_0298 "Roll (horizon) step"
#define UFC_STRING_RESOURCE_0301 "Distance from origin"
#define UFC_STRING_RESOURCE_0302 "Field of view"
#define UFC_STRING_RESOURCE_0303 "Roll (horizon)"
#define UFC_STRING_RESOURCE_0304 "Center of projection (X)"
#define UFC_STRING_RESOURCE_0305 "Initialization"
#define UFC_STRING_RESOURCE_0306 "Refinement"
#define UFC_STRING_RESOURCE_0307 "User-defined"
#define UFC_STRING_RESOURCE_0308 "Preset"
#define UFC_STRING_RESOURCE_0310 "Open"
#define UFC_STRING_RESOURCE_0311 "Frame from disk..."
#define UFC_STRING_RESOURCE_0312 "Open file"
#define UFC_STRING_RESOURCE_0313 "Failed to open file"
#define UFC_STRING_RESOURCE_0314 "Frame from Webcam..."
#define UFC_STRING_RESOURCE_0324 "Iteration settings"
#define UFC_STRING_RESOURCE_0325 "Camera settings"
#define UFC_STRING_RESOURCE_0402 "Close"
#define UFC_STRING_RESOURCE_0414 "Failed to download the camera settings for this play. Please verify your connection settings."
#define UFC_STRING_RESOURCE_0422 "./data/extrinsic/"
#define UFC_STRING_RESOURCE_0423 "./data/camera/"
#define UFC_STRING_RESOURCE_0431 "./data/camera/CAMERA_ID.json"
#define UFC_STRING_RESOURCE_0433 "./data/extrinsic/CAMERA_ID.json"
#define UFC_STRING_RESOURCE_0465 "Center of projection (Y)"
#define UFC_STRING_RESOURCE_0466 "Center of projection (Z)"
#define UFC_STRING_RESOURCE_0468 "Failed to update camera settings. It might be necessary to right-click on this play and ask for a \"reload\" of play settings."
#define UFC_STRING_RESOURCE_0487 "Left"
#define UFC_STRING_RESOURCE_0488 "Up"
#define UFC_STRING_RESOURCE_0489 "Right"
#define UFC_STRING_RESOURCE_0490 "Down"
#define UFC_STRING_RESOURCE_0491 "Space"
#define UFC_STRING_RESOURCE_0492 "+"
#define UFC_STRING_RESOURCE_0493 ","
#define UFC_STRING_RESOURCE_0494 "-"
#define UFC_STRING_RESOURCE_0495 "."
#define UFC_STRING_RESOURCE_0496 "/"
#define UFC_STRING_RESOURCE_0497 "0"
#define UFC_STRING_RESOURCE_0498 "1"
#define UFC_STRING_RESOURCE_0499 "2"
#define UFC_STRING_RESOURCE_0500 "3"
#define UFC_STRING_RESOURCE_0501 "4"
#define UFC_STRING_RESOURCE_0502 "5"
#define UFC_STRING_RESOURCE_0503 "6"
#define UFC_STRING_RESOURCE_0504 "7"
#define UFC_STRING_RESOURCE_0505 "8"
#define UFC_STRING_RESOURCE_0506 "9"
#define UFC_STRING_RESOURCE_0507 ";"
#define UFC_STRING_RESOURCE_0508 "A"
#define UFC_STRING_RESOURCE_0509 "B"
#define UFC_STRING_RESOURCE_0510 "C"
#define UFC_STRING_RESOURCE_0511 "D"
#define UFC_STRING_RESOURCE_0512 "E"
#define UFC_STRING_RESOURCE_0513 "F"
#define UFC_STRING_RESOURCE_0514 "G"
#define UFC_STRING_RESOURCE_0515 "H"
#define UFC_STRING_RESOURCE_0516 "I"
#define UFC_STRING_RESOURCE_0517 "J"
#define UFC_STRING_RESOURCE_0518 "K"
#define UFC_STRING_RESOURCE_0519 "L"
#define UFC_STRING_RESOURCE_0520 "M"
#define UFC_STRING_RESOURCE_0521 "N"
#define UFC_STRING_RESOURCE_0522 "O"
#define UFC_STRING_RESOURCE_0523 "P"
#define UFC_STRING_RESOURCE_0524 "Q"
#define UFC_STRING_RESOURCE_0525 "R"
#define UFC_STRING_RESOURCE_0526 "S"
#define UFC_STRING_RESOURCE_0527 "T"
#define UFC_STRING_RESOURCE_0528 "U"
#define UFC_STRING_RESOURCE_0529 "V"
#define UFC_STRING_RESOURCE_0530 "W"
#define UFC_STRING_RESOURCE_0531 "X"
#define UFC_STRING_RESOURCE_0532 "Y"
#define UFC_STRING_RESOURCE_0533 "Z"
#define UFC_STRING_RESOURCE_0534 "["
#define UFC_STRING_RESOURCE_0535 "\\"
#define UFC_STRING_RESOURCE_0536 "]"
#define UFC_STRING_RESOURCE_0537 "Key assignment"
#define UFC_STRING_RESOURCE_0540 "Play video"
#define UFC_STRING_RESOURCE_0541 "Previous frame"
#define UFC_STRING_RESOURCE_0542 "Next frame"
#define UFC_STRING_RESOURCE_0553 "="
#define UFC_STRING_RESOURCE_0554 "Open INTRINSIC calibration file"

#endif // #if !defined(STRING_RESOURCE_INCLUDED)


