# ShooterGame

Shooter Game is an Unreal Engine 4 C++ Third Person Shooter Game.

This project was created on July 8th, 2020. It was created and managed on my local GitLab server. Unfortunately, I was not able to directly port the GitLab project over to GitHub as it was both locally hosted, and was in excess of 20GB (the only files included here are the C#, cpp and header source files).


Below is the Patch and Update History.

**---------- 12 Oct, 2020
Update 0.000.90:**
- Fixed C++ header errors (I included header files multiple times in one header/cpp file set)



**---------- 04 Oct, 2020
Patch 0.000.85:**
- Uploading files for new computer configuration.



**---------- 27 Sep, 2020
Update 0.000.80:**
- Fixed Animation bugs
- Added Smooth Crouch
- Added Custom Movement Component



**---------- 07 Sep, 2020
Update 0.000.70:**
- Added a new Landscape Material
- Added Landscape Tiling and Distance Based Tiling
- Fixed Inventory System Bugs


**---------- 30 Aug, 2020
Update 0.000.60:**
- Added Pump Shotgun
- Moved Gun logic to AGunBase class
- Moved AnimBP Tick event from Blueprint to C++


**---------- 25 Aug, 2020
Update 0.000.50:**
- Fixed random bugs that caused crashing (Put UPROPERTY tags on top of variables to exempt from garbage collection)
- Fixed Multiplayer bugs (Now fully works in multiplayer)
- Fixed Multiplayer Lag
- 1st Update Since Vacation


**---------- 07 Aug, 2020
Patch 0.000.45:**
- Pushed This Update to Sync With Mac
- Added Volumetric Clouds
- Fixed up the Widgets


**---------- 04 Aug, 2020
Update 0.000.40:**
- Fixed Major Inventory Bugs
- Moved All Widgets Into HUDComponent Class
- Minor UI Tweaking


**---------- 29 Jul, 2020
Patch 0.000.35:**
- Removed Tick Event From InventoryWidget
- Added Different Min and Max Sized to Reticle When Aimed In
- Fixed Reloading Bug


**---------- 27 Jul, 2020
Update 0.000.30:**
- Recreated the Inventory System
- Edited Interp Speed of Reticle
- Edited the Disappearance Speed of the Hit Reticle
- Fixed Bug That Didn't Show Reticle OnBeginPlay


**---------- 25 Jul, 2020
Patch 0.000.25:**
- Added New Sound For M4A1
- Added A ShooterGameHUD Class Which Manages The Widgets On The Screen
- Moved Widget Code From Player Controller to ShooterGameHUD


**---------- 24 Jul, 2020
Update 0.000.20:**
- Refined Inventory UI
- Added Reticle UI Bloom (Though Still Doesn't Have a Real Function)


**---------- 16 Jul, 2020
Update 0.000.10:**
- Added the Inventory System
- Fixed up Inventory Component to Support Multi-Gun Inventories


**---------- 12 Jul, 2020
Patch 0.000.06:**
- Fixed Inverse Kinematics in Blueprint.
- Disabled Inverse Kinematic features while player is crouched


**---------- 11 Jul, 2020
Massive Aesthetic Overhaul:**
- Added attenuation audio effects to the AK47 fire sound.
- Added Inverse Kinematics for the character's feet in blueprint.
- Fixed a vital replication bug that ran the weapon fire code multiple times.


**---------- 10 Jul, 2020**
Replication:
- Replicated all of the main character movements and the weapon capabilities


**---------- 08 Jul, 2020
AI and GameMode Revamp:**
- Refined AI Movements
- Added Wave Based Spawning
- Added functionality to the ShooterGameGameMode class

**Refined AI Movements:**
- Added Wave Spawning for AI
- Added to ShooterGameGameMode

**Health Revamp:**
- Ported the UMG HealthWidget blueprint to C++ code.
- Added a health animation when a shooter game character loses all health.
- Added a check if the AI is dead it will stop searching for the player.

**Initial Commit:**
- The initial commit of adding all of the files to the repo.
