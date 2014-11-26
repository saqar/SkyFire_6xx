DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_mana_burn';

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_shadowy_insight';
INSERT INTO `spell_script_names` VALUES (124430, 'spell_pri_shadowy_insight');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_mind_blast';
INSERT INTO `spell_script_names` VALUES (8092, 'spell_pri_mind_blast');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_devouring_plague';
INSERT INTO `spell_script_names` VALUES (2944, 'spell_pri_devouring_plague');
