@tool
extends HBoxContainer

var _file_dialog:FileDialog ;

# create the file browser for later
func _enter_tree() ->void:
	_file_dialog =  FileDialog.new();
	_file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_DIR;
	
	_file_dialog.connect("file_selected", file_selected)

	
	var base_control = EditorInterface.get_base_control()
	base_control.add_child(_file_dialog)


func _exit_tree():
	# Cleanup
	_file_dialog.queue_free()
	
func show_dialog():
	print("show dialog");
	_file_dialog.popup_centered_ratio();
	
func file_selected(path):
	$pathText.text = path
