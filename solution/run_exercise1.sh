#!/bin/sh
( cd .. ; ./solution/bin/linux/cgv_viewer plugin:cg_fltk "type(shader_config):shader_path='./solution/glsl'" plugin:cg_icons plugin:cg_ext plugin:cg_meta plugin:cmi_io plugin:crg_stereo_view plugin:CG2_exercise2 config:"./solution/config_ex1.cfg" )
