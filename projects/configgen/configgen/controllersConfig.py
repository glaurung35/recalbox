#!/usr/bin/env python
import xml.etree.ElementTree as ET
import recalboxFiles

esInputs = recalboxFiles.esInputs


class Input:
    def __init__(self, name, type_, id_, value, code):
        self.name = name
        self.type = type_
        self.id = id_
        self.value = value
        self.code = code

    def clone(self):
        return Input(self.name, self.type, self.id, self.value, self.code)


class Controller:
    def __init__(self, configName, type_, guid, player, index="-1", realName="", inputs=None, dev=None, nbaxes=None, nbhats=None, nbbuttons=None):
        self.type = type_
        self.configName = configName
        self.index = index
        self.realName = realName
        self.guid = guid
        self.player = player
        self.dev = dev
        self.nbaxes = nbaxes
        self.nbhats = nbhats
        self.nbbuttons = nbbuttons
        if inputs is None:
            self.inputs = dict()
            self.axisesNumber = []
        else:
            self.inputs = inputs
            self.axisesNumber = self.setAxisNumberList()

    def generateSDLGameDBLine(self):
        # Making a dirty assumption here : if a dpad is an axis, then it shouldn't have any analog joystick
        nameMapping = {
            'a'             : { 'button' : 'b' },
            'b'             : { 'button' : 'a' },
            'x'             : { 'button' : 'y' },
            'y'             : { 'button' : 'x' },
            'start'         : { 'button' : 'start' },
            'select'        : { 'button' : 'back' },
            'l1'            : { 'button' : 'leftshoulder' },
            'r1'            : { 'button' : 'rightshoulder' },
            'l2'            : { 'button' : 'lefttrigger',  'axis' : 'lefttrigger' },
            'r2'            : { 'button' : 'righttrigger', 'axis' : 'righttrigger' },
            'l3'            : { 'button' : 'leftstick' },
            'r3'            : { 'button' : 'rightstick' },
            'up'            : { 'button' : 'dpup',    'hat' : 'dpup',   'axis' : 'lefty' },
            'down'          : { 'button' : 'dpdown',  'hat' : 'dpdown' },
            'left'          : { 'button' : 'dpleft',  'hat' : 'dpleft', 'axis' : 'leftx' },
            'right'         : { 'button' : 'dpright', 'hat' : 'dpright' },
            'joystick1up'   : { 'axis' : 'lefty' },
            'joystick1left' : { 'axis' : 'leftx' },
            'joystick2up'   : { 'axis' : 'righty' },
            'joystick2left' : { 'axis' : 'rightx' },
            'hotkey'        : { 'button' : 'guide' }
        }
        nameMappingForceHatToAxis = {
            'up'            : { 'button' : '-lefty', 'hat' : '-lefty' },
            'down'          : { 'button' : '+lefty', 'hat' : '+lefty' },
            'left'          : { 'button' : '-leftx', 'hat' : '-leftx' },
            'right'         : { 'button' : '+leftx', 'hat' : '+leftx' }
        }
        typePrefix = {
            'axis'   : 'a',
            'button' : 'b',
            'hat'    : 'h0.' # Force dpad 0 until ES handles others
        }
        
        if not self.inputs:
            return None

        # If no left axis defined, map hat to left axis
        if not self.inputs.has_key('joystick1up'):
            nameMapping.update(nameMappingForceHatToAxis)
            
        # Need to remove commas from the device name
        strOut = "{},{},platform:Linux,".format(self.guid, self.configName.replace(',', ' '))
        
        for idx, inp in self.inputs.iteritems():
            if inp.name in nameMapping and inp.type in typePrefix and inp.type in nameMapping[inp.name] :
                if inp.type == 'hat':
                    strOut += "{}:{}{},".format(nameMapping[inp.name][inp.type], typePrefix[inp.type], inp.value)
                else:
                    strOut += "{}:{}{},".format(nameMapping[inp.name][inp.type], typePrefix[inp.type], inp.id)
        
        return strOut

    def count(self, inputType):
        count = 0
        for idx, inp in self.inputs.iteritems():
            if inp.type == inputType:
                count += 1
        return count

    def setAxisNumberList(self):
        # First let's find and sort all those available axes in the controller
        # Remember : a pad can have 6 axes and 1 hat, but only 4 axes are mapped
        # All those tricks are just to mimic https://github.com/xbmc/peripheral.joystick/blob/master/src/api/udev/JoystickUdev.cpp#L321-L334
        axises = []
        for idx, inp in self.inputs.iteritems():
            if inp.type == 'axis':
                # As of now our patched SDL2 gives the same axes number to up/down and left/right. But it's wrong
                leftcode = self.inputs["left"].code if "left" in self.inputs else -1
                if inp.name in ['up', 'down'] and inp.code == leftcode:
                    code = int(inp.code) + 1
                else:
                    code = int(inp.code)
                if code not in axises:
                    axises.append(code)
        axises.sort()
        # Complete with existing axes but not in es_input.cfg
        # In the case of a 6 axes pad, complete with the 2 unassigned axes
        for i in range(len(axises), int(self.nbaxes)):
            axises.append(None)

        # Now add hats because they are after all axes
        hats = []
        for idx, inp in self.inputs.iteritems():
            if inp.type == 'hat':
                # As of now our patched SDL2 gives the same axes number to up/down and left/right. But it's wrong
                leftcode = self.inputs["left"].code if "left" in self.inputs else -1
                if inp.name in ['up', 'down'] and inp.code == leftcode:
                    code = int(inp.code) + 1
                else:
                    code = int(inp.code)
                if code not in hats:
                    hats.append(code)
        hats.sort()
        # Time to sort this
        axises.extend(hats)
        return axises

    def getAxisNumber(self, inp):
        # ~ print self.axisesNumber
        if inp.type not in ['axis', 'hat']:
            return None
        if inp.type == "hat":
            if inp.name in ['up', 'down'] and inp.code == self.inputs["left"].code:
                code = int(inp.code) + 1
                # ~ return int(self.nbaxes) + 1
            else:
                code = int(inp.code)
                # ~ return int(self.nbaxes)
        else:
            code = int(inp.code)
        return self.axisesNumber.index(code)

    def getTotalAxisNumber(self):
        return int(self.nbaxes) + 2*int(self.nbhats)

    # Load all controllers from the es_input.cfg
    @staticmethod
    def loadAllControllersConfig():
        controllers = dict()
        tree = ET.parse(esInputs)
        root = tree.getroot()
        for controller in root.findall(".//inputConfig"):
            controllerInstance = Controller(controller.get("deviceName"), controller.get("type"),
                                            controller.get("deviceGUID"), "0",
                                            inputs=None,
                                            dev=None,
                                            nbaxes=controller.get("deviceNbAxes"),
                                            nbhats=controller.get("deviceNbHats"),
                                            nbbuttons=controller.get("deviceNbButtons"))
            uidname = controller.get("deviceGUID") + controller.get("deviceName")
            controllers[uidname] = controllerInstance
            for inp in controller.findall("input"):
                name = inp.get("name")
                if name == 'pageup': name = 'l1'
                if name == 'pagedown': name = 'r1'
                inputInstance = Input(name, inp.get("type"), inp.get("id"), inp.get("value"), inp.get("code"))
                controllerInstance.inputs[name] = inputInstance
        return controllers

    # Load all controllers from the es_input.cfg
    @staticmethod
    def loadAllControllersByNameConfig():
        controllers = dict()
        tree = ET.parse(esInputs)
        root = tree.getroot()
        for controller in root.findall(".//inputConfig"):
            controllerInstance = Controller(controller.get("deviceName"), controller.get("type"),
                                            controller.get("deviceGUID"), "0",
                                            inputs=None,
                                            dev=None,
                                            nbaxes=controller.get("deviceNbAxes"),
                                            nbhats=controller.get("deviceNbHats"),
                                            nbbuttons=controller.get("deviceNbButtons"))
            deviceName = controller.get("deviceName")
            controllers[deviceName] = controllerInstance
            for inp in controller.findall("input"):
                name = inp.get("name")
                if name == 'pageup': name = 'l1'
                if name == 'pagedown': name = 'r1'
                inputInstance = Input(name, inp.get("type"), inp.get("id"), inp.get("value"), inp.get("code"))
                controllerInstance.inputs[name] = inputInstance
        return controllers


    # Get a device/start code map, for demo mode
    @staticmethod
    def loadDemoConfig(p1index, p1guid, p1name, p1dev, p1nbaxes, 
            p2index, p2guid, p2name, p2dev, p2nbaxes, 
            p3index, p3guid, p3name, p3dev, p3nbaxes,
            p4index, p4guid, p4name, p4dev, p4nbaxes, 
            p5index, p5guid, p5name, p5dev, p5nbaxes,
            p6index, p6guid, p6name, p6dev, p6nbaxes,
            p7index, p7guid, p7name, p7dev, p7nbaxes,
            p8index, p8guid, p8name, p8dev, p8nbaxes,
            p9index, p9guid, p9name, p9dev, p9nbaxes,
            p10index, p10guid, p10name, p10dev, p10nbaxes):
        result = dict()
        controllers = Controller.loadAllControllersConfig()

        newController = Controller.findBestControllerConfig(controllers, '1', p1guid, p1index, p1name, p1dev, p1nbaxes)
        if newController:
            result[p1dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '2', p2guid, p2index, p2name, p2dev, p2nbaxes)
        if newController:
            result[p2dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '3', p3guid, p3index, p3name, p3dev, p3nbaxes)
        if newController:
            result[p3dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '4', p4guid, p4index, p4name, p4dev, p4nbaxes)
        if newController:
            result[p4dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '5', p5guid, p5index, p5name, p5dev, p5nbaxes)
        if newController:
            result[p5dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '6', p6guid, p6index, p6name, p6dev, p6nbaxes)
        if newController:
            result[p6dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '7', p7guid, p7index, p7name, p7dev, p7nbaxes)
        if newController:
            result[p7dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '8', p8guid, p8index, p8name, p8dev, p8nbaxes)
        if newController:
            result[p8dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '9', p9guid, p9index, p9name, p9dev, p9nbaxes)
        if newController:
            result[p9dev] = newController.inputs["start"].code
        newController = Controller.findBestControllerConfig(controllers, '10', p10guid, p10index, p10name, p10dev, p10nbaxes)
        if newController:
            result[p10dev] = newController.inputs["start"].code
        return result


    # Create a controller array with the player id as a key
    @staticmethod
    def loadControllerConfig(p1index, p1guid, p1name, p1dev, p1nbaxes, 
            p2index, p2guid, p2name, p2dev, p2nbaxes, 
            p3index, p3guid, p3name, p3dev, p3nbaxes,
            p4index, p4guid, p4name, p4dev, p4nbaxes, 
            p5index, p5guid, p5name, p5dev, p5nbaxes,
            p6index, p6guid, p6name, p6dev, p6nbaxes,
            p7index, p7guid, p7name, p7dev, p7nbaxes,
            p8index, p8guid, p8name, p8dev, p8nbaxes,
            p9index, p9guid, p9name, p9dev, p9nbaxes,
            p10index, p10guid, p10name, p10dev, p10nbaxes):
        playerControllers = dict()
        controllers = Controller.loadAllControllersConfig()

        newController = Controller.findBestControllerConfig(controllers, '1', p1guid, p1index, p1name, p1dev, p1nbaxes)
        if newController:
            playerControllers["1"] = newController
        newController = Controller.findBestControllerConfig(controllers, '2', p2guid, p2index, p2name, p2dev, p2nbaxes)
        if newController:
            playerControllers["2"] = newController
        newController = Controller.findBestControllerConfig(controllers, '3', p3guid, p3index, p3name, p3dev, p3nbaxes)
        if newController:
            playerControllers["3"] = newController
        newController = Controller.findBestControllerConfig(controllers, '4', p4guid, p4index, p4name, p4dev, p4nbaxes)
        if newController:
            playerControllers["4"] = newController
        newController = Controller.findBestControllerConfig(controllers, '5', p5guid, p5index, p5name, p5dev, p5nbaxes)
        if newController:
            playerControllers["5"] = newController
        newController = Controller.findBestControllerConfig(controllers, '6', p6guid, p6index, p6name, p6dev, p6nbaxes)
        if newController:
            playerControllers["6"] = newController
        newController = Controller.findBestControllerConfig(controllers, '7', p7guid, p7index, p7name, p7dev, p7nbaxes)
        if newController:
            playerControllers["7"] = newController
        newController = Controller.findBestControllerConfig(controllers, '8', p8guid, p8index, p8name, p8dev, p8nbaxes)
        if newController:
            playerControllers["8"] = newController
        newController = Controller.findBestControllerConfig(controllers, '9', p9guid, p9index, p9name, p9dev, p9nbaxes)
        if newController:
            playerControllers["9"] = newController
        newController = Controller.findBestControllerConfig(controllers, '10', p10guid, p10index, p10name, p10dev, p10nbaxes)
        if newController:
            playerControllers["10"] = newController
        return playerControllers

    @staticmethod
    def loadControllerConfig2(**kwargs):
        playerControllers = dict()
        controllers = Controller.loadAllControllersConfig()

        for i in range(1, 10):
            num = str(i)
            pguid = kwargs.get('p{}guid'.format(num), None)
            pindex = kwargs.get('p{}index'.format(num), None)
            pname = kwargs.get('p{}name'.format(num), None)
            pdev = kwargs.get('p{}dev'.format(num), None)
            pnbaxes = kwargs.get('p{}nbaxes'.format(num), None)
            if pdev is None:
                pdev = kwargs.get('p{}devicepath'.format(num), None)
            newController = Controller.findBestControllerConfig(controllers, num, pguid, pindex, pname, pdev, pnbaxes)
            if newController:
                playerControllers[num] = newController
        return playerControllers

    @staticmethod
    def findBestControllerConfig(controllers, x, pxguid, pxindex, pxname, pxdev, pxnbaxes):
        # when there will have more joysticks, use hash tables
        for controllerGUID in controllers:
            controller = controllers[controllerGUID]
            if controller.guid == pxguid and controller.configName == pxname:
                return Controller(controller.configName, controller.type, controller.guid, x, pxindex, pxname,
                                  controller.inputs, pxdev, controller.nbaxes, controller.nbhats, controller.nbbuttons)
        for controllerGUID in controllers:
            controller = controllers[controllerGUID]
            if controller.guid == pxguid:
                return Controller(controller.configName, controller.type, controller.guid, x, pxindex, pxname,
                                  controller.inputs, pxdev, pxnbaxes, controller.nbhats, controller.nbbuttons)
        for controllerGUID in controllers:
            controller = controllers[controllerGUID]
            if controller.configName == pxname:
                return Controller(controller.configName, controller.type, pxguid, x, pxindex, pxname,
                                  controller.inputs, pxdev, pxnbaxes, controller.nbhats, controller.nbbuttons)
        return None

    @staticmethod
    def generateSDLGameDBAllControllers(controllers, outputFile = "/tmp/gamecontrollerdb.txt"):
        finalData = ["# Controllers defined in Recalbox"]
        for idx, controller in controllers.iteritems():
            finalData.append(controller.generateSDLGameDBLine())
        sdlData = "\n".join(finalData)
        with open(outputFile, "w") as text_file:
            text_file.write(sdlData)
        return outputFile
