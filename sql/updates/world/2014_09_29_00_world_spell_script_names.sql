DELETE FROM spell_script_names WHERE spell_id IN(114049, 73920);
INSERT INTO spell_script_names VALUES (114049, "spell_sha_ascendance");
INSERT INTO spell_script_names VALUES (73920, "spell_sha_healing_rain");

DELETE FROM spell_linked_spell WHERE spell_trigger IN (165339, 165341, 165344);
INSERT INTO spell_linked_spell VALUES
(165339, 114050, 0, "Ascendance - Elemental"),
(165341, 114051, 0, "Ascendance - Enhancement"),
(165344, 114052, 0, "Ascendance - Restoration");