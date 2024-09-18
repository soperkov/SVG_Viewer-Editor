
![INK MUSE Ui](/Icons/gitPicture.jfif)

# INK MUSE
INK MUSE is a simple SVG viewer, editor, and creator. It offers an intuitive interface with basic tools for editing and creating SVG images, making it a perfect fit for both beginners and those looking for lightweight SVG manipulation.

## Features
### Viewer Mode
 * SVG Viewing: As the name suggests, the Viewer tab is for viewing SVG files. All editing controls are disabled, allowing you to focus on visualizing your image without distractions.

### Editor Mode
 * Shape Tools: Contains more than enough of shapes to make simble creations
 * Brush and Pen Controls: Customize outline color, fill color and size of your shapes using simple brush and pen settings.
 * Object Manipulation: Move, resize, and delete objects directly on the canvas. With keyboard object control you can be both fast and precise.
 * Zoom: Zoom in and out to fine-tune details or get a broad overview of your image.
 * Print & Print Preview: Support for printing and print preview, allowing you to bring your SVG creations to life on paper.

## Installation
### Prerequisites
 * Download and install Qt (version 6.7 is recommended).

### Steps
 1. Clone or download the project repository.
 2. Open the project in Qt Creator.
 3. Configure the project according to your environment.
 4. Build and run the application.

### Additional information
If building through Visual Studio:
 1. Install Qt with Visual Studio Support.
 2. Open the Developer Command Prompt and run:
    * cd path/to/your/pro/file
    * qmake -tp vc your_project.pro
    * That should generate .vcxproj file and project is ready for opening in Visual Studio.
 3. In project Properties: Configuration Propeties > C/C++ > General > Additional Include Directories > "[Qt
folder]\6.7.2\msvc2019_64\include\QtSvgWidgets".

Everything is set to run INK MUSE.
