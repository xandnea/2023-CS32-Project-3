# 2023-CS32-Project-3

1.
  I have two PlayerAvatar pointers called peach and yoshi that I pass to the bank square. The bank
  square will then check if peach or yoshi's X and Y coordinates are the same and that they are 
  directly on a square (due to integer division causing multiple pixel coordinates to give the same
  location) in its doSomething() function. I also included boolean private member variables for all 
  squares called peachOn and yoshiOn that were set to true if Peach or Yoshi were on a specific 
  square and would set to false if they weren't. Before the bank square acted, it would make sure 
  that peachOn and/or yoshiOn (depending on which player was detected) was false. If this wasn't 
  added in, the bank (or any other square) would run their actions (like blue squares giving coins) 
  repeatedly until the player moved off the square. After checking peachOn and/or yoshiOn, the bank 
  square checked the state of the player and gave or took coins depending on if it was WALKING or 
  WAITING_TO_ROLL. The only functions called in bank square are:
 
  - isPeachOn() and isYoshiOn() which returned the peachOn and yoshiOn booleans
  - checkIfLandedOn(character pointer) which is a square function that compares positions and ensures 
    the character is directly on a square
  - getState() which gets the state of the player
  - getNumCoins() which gets the coins of the player
  - addCoins(num coins added or subtracted) which changes the player's coins
  - addToBank(num coins added or subtracted) which changes the bank in student world 
  - playSound(sound played) which plays a sound from student world 
  - setBank(0) which sets the bank of student world to 0
  - setPeachOn(boolean) and setYoshiOn(boolean) which set peachOn and yoshiOn to true or false 
    depending on when it was called

2. (everything was implemented)
  - Bug: If boo/bowser land on a player, they won't move until the player moves.
  - Bug: Sometimes after a long game bowser will randomly delete a square, not place a dropping square,
  and randomly teleport.

3.
  - I set the players to start off with 0 coins
