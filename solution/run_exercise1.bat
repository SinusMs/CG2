@ECHO OFF
cd ..
.\solution\windows\cgv_viewer64_142.exe plugin:cg_fltk "type(shader_config):shader_path='%cd%\solution\glsl'" plugin:crg_stereo_view plugin:cg_icons plugin:cg_ext plugin:cmi_io plugin:CG2_exercise1 "config:'%cd%\solution\config_ex1.cfg'"
