# iotSwitch

This is the first slave module for the iotinator project, its purpose is to allow commuting a relay through the Xiot framework, in order to turn on and off a lamp, a heater, an air conditionner or any other device.

To use it, you'll preferrably also need the iotinator master module (https://github.com/reivaxy/iotinator), or you can just use it's REST API to integrate to your own designs.

Although it's functionnal, it's also intended to be a simple stub to create more complex slave modules, which is while it offers a simple feature, and won't be expanded or enriched with more features, like programmable scheduling for instance (this will be available in just another Xiot module)

You'll need to clone these repositories into your arduino "libraries" directory to be able to compile:

- https://github.com/reivaxy/XOLEDDisplay.git 
- https://github.com/reivaxy/XIOTDisplay.git
- https://github.com/reivaxy/XIOTConfig.git
- https://github.com/reivaxy/XEEPROMConfig.git
- https://github.com/reivaxy/XUtils.git
- https://github.com/reivaxy/XIOTModule.git


And some libraries that can be found through the Arduino IDE

TODO: list the libraries :)
 
 
