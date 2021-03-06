-- Sha-Infested Prowler SAI
SET @ENTRY := 66668;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,5000,15000,10000,20000,11,115083,0,0,0,0,0,2,0,0,0,0,0,0,0,"Sha-Infested Prowler - In Combat - Ferocious Claw");

-- Sha-Infested Prowler Creature Template Addon
DELETE FROM `creature_template_addon` WHERE entry=66668;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `auras`) VALUES
(66668, 0, 0x0, 0x1, '128906'); -- Sha-Corrupted