// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

/**
 * @title TurnLed
 * @dev Smart contract to control an LED state and emit corresponding events.
 */
contract TurnLed {
    /// @notice Current LED state: true = ON, false = OFF
    bool public isOn;

    /// @notice Emitted when the LED is turned ON
    event TurnedOn(address indexed sender);

    /// @notice Emitted when the LED is turned OFF
    event TurnedOff(address indexed sender);

    /**
     * @notice Turns the LED ON if it is OFF
     */
    function turnOn() external {
        if (isOn) revert AlreadyOn();
        isOn = true;
        emit TurnedOn(msg.sender);
    }

    /**
     * @notice Turns the LED OFF if it is ON
     */
    function turnOff() external {
        if (!isOn) revert AlreadyOff();
        isOn = false;
        emit TurnedOff(msg.sender);
    }

    /**
     * @notice Custom error when trying to turn on an already ON LED
     */
    error AlreadyOn();

    /**
     * @notice Custom error when trying to turn off an already OFF LED
     */
    error AlreadyOff();
}
