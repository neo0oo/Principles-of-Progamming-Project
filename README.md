Options per room:

    1: 4 (actually 3, with one conditional)
    2: 6 (but intended to be sequential)
    3: 6
    4: 5

Principles-of-Progamming-Project

How the game works (as per the C code)

    You start with 3 items in your inventory: Rope, Torch, Knife.
    Your health starts at 100.
    Each room presents descriptive text and a list of actions you can take. Some actions may require specific items from your inventory to be available.
    In the starting room, outside the cave, you see a cryptic message inscribed in stone. This has a hint for each room to pass without taking damage.
    You do not pick up new items to replace existing ones after rooms 1, 2, or 3 as initially planned. Inventory management is limited to the starting items unless an option explicitly states an item is given (which is not the case for rooms 1-4 in the provided code).
    The game progresses from room to room based on your choices. Some choices might keep you in the same room to try again.
    The last room (Room 5, following Room 4) has a boss fight.
    If your health drops to 0 or below at any point (either in a room or during the boss fight), you die, and the game restarts from the very beginning.
    The point of the game is to learn the patterns and correct choices to pass each room and defeat the boss to complete the game.
    Your final health will be displayed if you win.

New stuff after presentation (Code Implementation Status)

    rooms as parameters all in one function: (Partially) The play_room function is generic, taking room data.
    grade at the end depending on health (user efficiency): Not implemented, but final health is shown.
    go back and forth between rooms: Not implemented; progression is generally forward.
    display items in inventory at the start of every room: Implemented.
    display both items in inventory and items to pick up at the end of each room: Inventory is displayed. Item pickup is tied to specific actions, not automatically at the end of a room. The described item offerings post-room are not in the code.

Start:

    Small introduction (character) - Narrative, assumed before game starts
    Small introduction (situation) - Cave of fortunes has a mysterious item - Narrative, assumed
    Days of journeying through the forest - Narrative, assumed
    Finally stumbles upon the cave of fortune - Narrative, assumed
    At the door there is a message etched into the stone:
        "Clue 1: Light reveals hidden paths."
        "Clue 2: In football, think 1-2-1."
        "Clue 3: Sharp tools stop the poison." (Note: Code doesn't have poison machine needing sharp tools; this clue seems to misalign with room 3's coded reality where Gasmask is best or Knife/Pickaxe for damage)
        "Clue 4: Speed outlasts the horde." (Note: Code uses "Holy Symbol" for no damage, not speed items)
    Will you enter? --> Player inputs 'y' to enter.
    Inventory: rope, torch, knife. Health: 100.

Room 1: A cavern with a high beam and wide chasm.

    Option 1: **Take damage + pass room** Rope Swing with Rope --> Lose 5 health. Proceed to Room 2.
    Option 2: **Nothing** Knife Strike air with Knife --> Nothing happens. Remain in Room 1.
    Option 3: **Activate other item **Torch Light torch to search --> The room is illuminated. You notice something about the wall. Remain in Room 1. (This enables Option 4)
    Option 4: **Pass room** (Only available after choosing Option 3) Pry wall after torch --> Secret path opens --> Pass the room with no damage taken. Proceed to Room 2. No items are offered or picked up in this room based on the code. If player health drops to 0, game restarts from the beginning.

Room 2: Three platforms ahead. Choose left or right step by step.
The clue "In football, think 1-2-1" suggests a pattern (likely Right, Left, Right). You must choose correctly three times. An incorrect step costs 10 health and keeps you in the room to try that step again (or forces a full restart if health is depleted).
The code presents all step options at once, but logically you'd address them sequentially:

    Step 1:
        Option: Step Left --> Lose 10 health. Remain to retry Step 1.
        Option: Step Right --> No damage. Proceed to choices for Step 2.
    Step 2 (assuming Step 1 Right was chosen):
        Option: Step Left --> No damage. Proceed to choices for Step 3.
        Option: Step Right --> Lose 10 health. Remain to retry Step 2.
    Step 3 (assuming Step 2 Left was chosen):
        Option: Step Left --> Lose 10 health. Remain to retry Step 3.
        Option: Step Right --> No damage. Proceed to Room 3. No items are offered or picked up in this room. If player health drops to 0, game restarts from the beginning.

Room 3: Room fills with toxic gas; rocks block the path.

    Option 1: Use Torch --> The gas ignites! Instant death. Game restarts.
    Option 2: Use Rope --> Nothing happens. Remain in Room 3. (Code indicates "Rope" is not explicitly required in inventory for this, but assumed).
    Option 3: Use Knife --> You try to disable a mechanism. Take 10 damage. Proceed to Room 4.
    Option 4: Use Pickaxe (if you had it, but you don't pick it up in Room 1 per code) --> You try to disable a mechanism. Take 10 damage. Proceed to Room 4.
    Option 5: Use Pickaxe after Knife (if you had Pickaxe) --> No damage. Proceed to Room 4. (This assumes sequential choices if both items were present and Knife was used in a prior action that kept you in the room).
    Option 6: Wear Gasmask (if you had it, but you don't pick it up in Room 1 per code) --> Takes no damage. Proceed to Room 4. The optimal path relies on items (Pickaxe, Gas Mask) that are not acquired in earlier rooms according to the code. With starting gear, using the Knife (Option 3) is a way through, taking damage. No items (Heavy machine gun, Speed shoes, Angelic wings) are offered here per the code. If player health drops to 0, game restarts from the beginning.

Room 4: A swarm of skeletal foes surrounds you.

    Option 1: Use Knife --> You fight them off but take 10 damage. Proceed to the Final Room (Boss).
    Option 2: Wave Torch --> The light agitates them further! Instant death. Game restarts.
    Option 3: Drink Glass of Milk --> This was a bad choice! Instant death. Game restarts. (Note: "Glass of Milk" is not a starting item or acquired item).
    Option 4: Use Pickaxe (if you had it) --> You fight them off but take 10 damage. Proceed to the Final Room (Boss).
    Option 5: Use Holy Symbol (if you had it, but it's not a starting/acquired item) --> The skeletons recoil! You pass unharmed. Proceed to the Final Room (Boss). Items like Machine Gun, Speed shoes, Wings, or Rope for this room are not implemented as usable options in the code. The "Holy Symbol" is the only coded no-damage solution, but it's not an obtainable item based on prior rooms. With starting gear, using the Knife (Option 1) is a way through, taking damage. The complex restart logic (restarting at previous room for MG) is not in the code. Any death results in a full game restart.

Final Room: Boss Fight

    The Boss Battle Begins! You have 5 seconds to react to each of the boss's attacks by typing "duck", "jump", or "block".
        Boss attacks include: "swings a sword" (dodge: duck), "throws a fireball" (dodge: jump), "charges at you" (dodge: block), "stabs low" (dodge: jump), "casts lightning" (dodge: block).
        If you fail to type the correct reaction in time or type the wrong one, you take damage (10-20 HP).
    If you successfully dodge, you get a chance to attack within 5 seconds by typing "slash" or "kick".
        A successful attack damages the boss (10-15 HP, with a potential bonus if your attack type is super effective against the boss's last move type).
        Failing to attack in time or typing an invalid command means you miss.
    If you don't dodge, you are recovering and cannot attack.
    Your Health and the Boss's Health (starts at 100) are displayed.
    The fight continues until your health is <= 0 (You died. Game Over. Restart from beginning) or the boss's health is <= 0.
    If you defeat the boss: "You defeated the boss! Victory!" then "Congratulations! You escaped with [current] health." The game ends. The narrative elements of gold, jewels, pressure plate, barrier, key, mysterious item, and cave collapse are not present in the coded boss encounter. It's purely a timed combat sequence.
