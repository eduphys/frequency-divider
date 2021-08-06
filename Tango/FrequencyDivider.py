# -*- coding: utf-8 -*-
#
# This file is part of the FrequencyDivider project
#
# GPL
#
# Distributed under the terms of the GPL license.
# See LICENSE.txt for more info.

""" FrequencyDivider

TangoServer to access the frequency divider and change the divider setting.
"""

# PyTango imports
import tango
from tango import DebugIt
from tango.server import run
from tango.server import Device, DeviceMeta
from tango.server import attribute, command
from tango.server import device_property
from tango import AttrQuality, DispLevel, DevState
from tango import AttrWriteType, PipeWriteType
# Additional import
# PROTECTED REGION ID(FrequencyDivider.additionnal_import) ENABLED START #
from SerialCom import SerialCom
# PROTECTED REGION END #    //  FrequencyDivider.additionnal_import

__all__ = ["FrequencyDivider", "main"]


class FrequencyDivider(Device):
    """
    TangoServer to access the frequency divider and change the divider setting.

    **Properties:**

    - Device Property
        port
            - Type:'DevString'
    """
    __metaclass__ = DeviceMeta
    # PROTECTED REGION ID(FrequencyDivider.class_variable) ENABLED START #
    # PROTECTED REGION END #    //  FrequencyDivider.class_variable

    # -----------------
    # Device Properties
    # -----------------

    port = device_property(
        dtype='DevString',
        mandatory=True
    )

    # ----------
    # Attributes
    # ----------

    Divider = attribute(
        dtype='DevShort',
        access=AttrWriteType.READ_WRITE,
        memorized=True,
    )

    # ---------------
    # General methods
    # ---------------

    def init_device(self):
        """Initialises the attributes and properties of the FrequencyDivider."""
        Device.init_device(self)
        # PROTECTED REGION ID(FrequencyDivider.init_device) ENABLED START #
        self.set_state(tango.DevState.STANDBY)
        self.com = SerialCom(self.port)
        
        try:
            self.com.open()
            self.com.read_divider()
        except tango.DevFailed:
            self.set_status('Serial port could not be opened')
            self.set_state(tango.DevState.FAULT)
            
        
        
       
        # PROTECTED REGION END #    //  FrequencyDivider.init_device

    def always_executed_hook(self):
        """Method always executed before any TANGO command is executed."""
        # PROTECTED REGION ID(FrequencyDivider.always_executed_hook) ENABLED START #
        if self.com.is_open():
            self.set_state(tango.DevState.ON)
        else:
            self.set_state(tango.DevState.STANDBY)
        # PROTECTED REGION END #    //  FrequencyDivider.always_executed_hook

    def delete_device(self):
        """Hook to delete resources allocated in init_device.

        This method allows for any memory or other resources allocated in the
        init_device method to be released.  This method is called by the device
        destructor and by the device Init command.
        """
        # PROTECTED REGION ID(FrequencyDivider.delete_device) ENABLED START #
        self.com.close()
        # PROTECTED REGION END #    //  FrequencyDivider.delete_device
    # ------------------
    # Attributes methods
    # ------------------

    def read_Divider(self):
        # PROTECTED REGION ID(FrequencyDivider.Divider_read) ENABLED START #
        """Return the Divider attribute."""
        
        return self.com.divider
        
        # PROTECTED REGION END #    //  FrequencyDivider.Divider_read

    def write_Divider(self, value):
        # PROTECTED REGION ID(FrequencyDivider.Divider_write) ENABLED START #
        """Set the Divider attribute."""
        if value < 1 or value > 32 or (value%2 != 0 and value != 1):
            self.set_status("Divider not valid")
            return
        
        self.com.set_divider(value)
        # PROTECTED REGION END #    //  FrequencyDivider.Divider_write

    def is_Divider_allowed(self, attr):
        # PROTECTED REGION ID(FrequencyDivider.is_Divider_allowed) ENABLED START #
        if attr==attr.READ_REQ:
            return self.get_state() not in [DevState.STANDBY]
        else:
            return self.get_state() not in [DevState.STANDBY]
        # PROTECTED REGION END #    //  FrequencyDivider.is_Divider_allowed

    # --------
    # Commands
    # --------

    @command(
    )
    @DebugIt()
    def Open(self):
        # PROTECTED REGION ID(FrequencyDivider.Open) ENABLED START #
        """
        Open port

        :return:None
        """
        self.com.open()
        # PROTECTED REGION END #    //  FrequencyDivider.Open

    @command(
    )
    @DebugIt()
    def Close(self):
        # PROTECTED REGION ID(FrequencyDivider.Close) ENABLED START #
        """
        Close port

        :return:None
        """
        self.com.close()
        # PROTECTED REGION END #    //  FrequencyDivider.Close

# ----------
# Run server
# ----------


def main(args=None, **kwargs):
    """Main function of the FrequencyDivider module."""
    # PROTECTED REGION ID(FrequencyDivider.main) ENABLED START #
    return run((FrequencyDivider,), args=args, **kwargs)
    # PROTECTED REGION END #    //  FrequencyDivider.main


if __name__ == '__main__':
    main()
