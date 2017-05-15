Some game using LiquidFun and SFML

Work in progress

Gameplay
========
* Serve drinks to customers
* All levels consist of a counter from the POV of a bartender
* Customers can order from a finite set of drinks one or more drinks
* The customer must be given the correct drink in reasonable time, else the approval rate of the player will fall
* When the approval rate reaches zero, the player is fired
* If the player serves enough customers, the player can transition to the next level (with more customers and trickier orders)
Ordering a drink:
* The drink ordered by a customer is shown
* The player must move an empty cup to the drink dispenser machine under the correct slot, press and hold the button to dispense the drink from the container and release the button so no overflow occurs
* When the drink is filled, it must be moved carefully to the customer
* The amount of liquid in the waste bin is deducted from the player's approval rating when the level finishes
* If the cup given to a customer is not full enough, the customer complains and the approval rate drops slightly
* If the cup contains the wrong drink, the customer complains and the approval rate drops sharply
* If the player spills liquid on the counter, all customers complain and the approval rate drops according to the amount of customers
* Careless handling of cups can make holes in them
* Making a mistake when filling a cup can be corrected by emptying the contents of the cup into a waste bin and refilling the cup
* The drink container may become empty and must be refilled
* The drink dispenser machine has an operating condition and if the condition falls too low, the machine breaks and sprays liquid everywhere
* The drink dispenser machine can be maintained easily (with a wrench) but after breaking, fixing it takes more time (with a hammer)
* Using a hammer to maintain the machine lowers the condition sharply

TODO
====
* Fill a cup with liquid
* Vertical platform (bar counter)
* Dispenser faucet
* Drink dispenser machine
* Transfer everything into SFML using placeholder sprites
* Infrastructure (menu and state management)
* Map game states to UI

