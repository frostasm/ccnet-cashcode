#ifndef COMMANDS_H
#define COMMANDS_H

// Controller Commands
enum class ValidatorCommands : unsigned char {
    Ack                     = 0x00,
    Nak                     = 0xFF,
    Reset                   = 0x30, // Command for Bill-to-Bill unit to self-reset
    GetStatus               = 0x31, // Request for Bill-to-Bill unit set-up status
    SetSecurity             = 0x32, // Sets Bill-to-Bill unit Security Mode. Command is followed by set-up data. See command format
    Poll                    = 0x33, // Request for Bill-to-Bill unit activity Status
    EnableBillTypes         = 0x34, // Indicates Bill Type enable or disable. Command is followed by set-up data. See command format
    Stack                   = 0x35, // Sent by Controller to stack a bill in escrow into drop cassette or into one of the recycling cassettes
    Return                  = 0x36, // Sent by Controller to return a bill in escrow
    Identification          = 0x37, // Request for Model, Serial Number, Software Version of Bill-to-Bill unit, Country ISO code, Asset Number
    Hold                    = 0x38, // Command for holding the Bill-to-Bill unit in Escrow state
    CassetteStatus          = 0x3B, // Request for Bill-to-Bill unit cassette status
    Dispense                = 0x3C, // Command to dispense a bill of specific type
    Unload                  = 0x3D, // Command to unload bills from recycling cassette(s) to drop cassette
    EscrowCassetteStatus    = 0x3E, // Request for recycling cassette(s) status
    EscrowCassetteUnload    = 0x3F, // Command for routing bills to recycling cassette(s)
    SetCassetteType         = 0x40, // Assigns cassettes to bill types
    GetBillTable            = 0x41, // Request for bill type assignments
    Download                = 0x50, // Command for transition to download mode.
    RequestStatistics       = 0x60, // Command for retrieving full information about acceptance performance.
};


enum class PollResponseType : unsigned char {
    Ack                 = 0x00,
    PowerUp             = 0x10, // The state of Bill-to-Bill unit after power up
    PowerBill           = 0x11, // Power up with bill in validating head. After a RESET command from the Controller,
                                // the Bill-to-Bill unit returns the bill and continues initializing.
    Initialize          = 0x13, // The state, in which Bill-to-Bill unit executes initialization after RESET command from the Controller.
    Idling              = 0x14, // In this state Bill-to-Bill unit waits for bill insertion.
    Accepting           = 0x15, // In this state Bill-to-Bill unit executes scanning of a bill and determines its denomination.
    Stacking            = 0x17, // In this state, the Bill-to-Bill unit transports a bill from Escrow position to the recycling cassette
                                // or to the drop box and remains in this state until the bill is stacked or returned if jammed.
    Returning           = 0x18, // In this state Bill-to-Bill unit transports a bill from Escrow position to entry bezel and
                                // remains in this state until the bill is removed by customer.
    UnitDisabled        = 0x19, // The Bill-to-Bill unit has been disabled by the Controller and also the state in which Bill-to-Bill unit is after initialization
    Rejecting           = 0x1C, // Generic rejecting code. Always followed by rejection reason byte (see below).
    BillValidatorJammed = 0x43, // Bill(s) are jammed in the acceptance path.
    EscrowPosition      = 0x80, // Events with credit - Escrow position
    BillStacked         = 0x81, // Events with credit - Bill stacked
    BillReturned        = 0x82, // Events with credit - Bill returned
};



#endif // COMMANDS_H
