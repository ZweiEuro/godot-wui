Project based on the excellent template godot-roguelite 
github.com:vorlac/godot-roguelite.git





# What is this?

This is a godot project and plugin for godot that shows usage of WUI. 

WUI is a UI rendering system that allows to use HTML UI inside games.

## What is a dev server
You have 2 choices to run your UI in WUI.
1. Statically created file
2. A live web server

In this example, the UI is inside `project/html` and compiles statically into `program/html/dist` when `npm run build` are run. A dev-server can be started with `npm run start`; which will host your website on `localhost:3000` and reload whenever something changes.

### Static files
Compiling any website statically (so that there is a valid `index.html`) that renders your page and putting the required files into a folder inside your project.
You can then directly navigate WUI there (absolute file paths, or starting with `res://` which gets resolved by the plugin internally).

This will mean your webpage will not hot-reload even if changes are made (and recompiled).

### A hot reloading serve

Webpack (and many similar systems) has a built-in dev-server. Starting it will make your website live-reload and host on some localhost port which WUI can be directed to. 

You can also direct WUI to any other webpage as long as its resolve-able; though bear in mind that the security and safety concerns are your own when doing this. WUI is a normal web-browser, but there is no force-https or similar safety feature. Navigating to unknown pages is not recommended and, frankly, not the point of this library.