// Sources flattened with hardhat v2.22.16 https://hardhat.org

// SPDX-License-Identifier: MIT

// File contracts/VendingMachine.sol

// Original license: SPDX_License_Identifier: MIT
pragma solidity ^0.8.0;

/*
 * @title VendingMachine
 * @dev A simple vending machine contract that allows the owner to add items and users to purchase them.
 * The contract keeps track of items and their prices, and allows the owner to withdraw the balance.
 */

// Address: 0xaeb3e98F0084963FB0255cA20017A3C252540a55 (Base Sepolia)
contract VendingMachine {
    address public owner;

    struct Item {
        string name;
        uint256 price;
        uint256 amount;
    }

    uint256 public itemCounter;
    mapping(uint256 => Item) public itemsByIndex;
    mapping(uint256 => uint256) public priceToIndex;

    // This event is listened by the ESP32 
    event ProductPurchased(address indexed buyer, uint256 index, string name, uint256 amount);

    modifier onlyOwner() {
        require(msg.sender == owner, "Not owner");
        _;
    }

    // Initializes the contract with default items.
    constructor() {
        owner = msg.sender;

        addItem("Gold", 0.01 ether, 1);
        addItem("Silver", 0.001 ether, 1);
        addItem("Beer", 0.0005 ether, 1);
        addItem("Popcorn", 0.0004 ether, 1);
    }

    function addItem(string memory name, uint256 price, uint amount) public onlyOwner {
        require(itemCounter < 255, "Item limit reached");
        require(price > 0, "Price must be greater than zero");
        require(bytes(name).length > 0, "Name cannot be empty");
        priceToIndex[price] = itemCounter;
        itemsByIndex[itemCounter] = Item(name, price, amount);
        itemCounter++;
    }

    function updateItem(uint256 index, string memory name, uint256 price, uint256 amount) public onlyOwner {
        require(index < itemCounter, "Index out of bounds");
        require(price > 0, "Price must be greater than zero");
        require(bytes(name).length > 0, "Name cannot be empty");
        itemsByIndex[index] = Item(name, price, amount);
    }

    function removeItem(uint256 index) public onlyOwner {
        require(index < itemCounter, "Index out of bounds");
        delete itemsByIndex[index];
        for (uint256 i = index; i < itemCounter - 1; i++) {
            itemsByIndex[i] = itemsByIndex[i + 1];
        }
        delete itemsByIndex[itemCounter - 1];
        itemCounter--;
    }

    // The ESP32 use function to load products into the machine.
    function getItem(uint256 index) external view returns (Item memory) {
        return itemsByIndex[index];
    }
    // ---------------------------------------------------------------------

    receive() external payable {
        uint256 productIndex = priceToIndex[msg.value];
        require(productIndex < itemCounter, "Invalid payment amount");

        Item memory item = itemsByIndex[productIndex];
        require(item.price == msg.value, "Invalid price match");
        require(item.amount > 0, "Item out of stock");
        itemsByIndex[productIndex].amount -= 1;

        emit ProductPurchased(msg.sender, productIndex, item.name, msg.value);
    }

    fallback() external payable {
        revert("Fallback not allowed");
    }

    function withdraw() external onlyOwner {
        uint256 amount = address(this).balance;
        require(amount > 0, "No balance to withdraw");
        (bool sent, ) = owner.call{value: amount}("");
        require(sent, "Withdraw failed");
    }

}
