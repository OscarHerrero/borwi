// SPDX-License-Identifier: MIT
pragma solidity ^0.8.24;

contract VendingMachine {
    address public owner;

    struct Item {
        string name;
        uint256 price;
        uint256 amount;
    }

    Item[] public items;
    mapping(uint256 => uint256) public priceToIndex;

    event ProductPurchased(address indexed buyer, uint256 index, string name, uint256 amount);

    modifier onlyOwner() {
        require(msg.sender == owner, "Not owner");
        _;
    }

    constructor() {
        owner = msg.sender;

        addItem("Gold", 0.01 ether, 1);
        addItem("Silver", 0.001 ether, 1);
        addItem("Beer", 0.0005 ether, 1);
        addItem("Popcorn", 0.0004 ether, 1);
    }

    function addItem(string memory name, uint256 price, uint256 amount) public onlyOwner {
        items.push(Item(name, price, amount));
        priceToIndex[price] = items.length - 1;
    }

    function getItemCount() external view returns (uint256) {
        return items.length;
    }

    function getItem(uint256 index) external view returns (string memory) {
        require(index < items.length, "Index out of bounds");
        return items[index].name;
    }

    function getItemPrice(uint256 index) external view returns (uint256) {
        require(index < items.length, "Index out of bounds");
        return items[index].price;
    }

    function getItemAmount(uint256 index) external view returns (uint256) {
        require(index < items.length, "Index out of bounds");
        return items[index].amount;
    }

    receive() external payable {
        uint256 index = priceToIndex[msg.value];
        require(index < items.length, "Invalid payment amount");

        Item memory item = items[index];
        require(item.price == msg.value, "Invalid price match");

        emit ProductPurchased(msg.sender, index, item.name, msg.value);
    }

    function withdraw() external onlyOwner {
        uint256 amount = address(this).balance;
        require(amount > 0, "No balance to withdraw");
        (bool sent, ) = owner.call{value: amount}("");
        require(sent, "Withdraw failed");
    }
}
