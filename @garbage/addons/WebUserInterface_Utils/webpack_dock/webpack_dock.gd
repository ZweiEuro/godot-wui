@tool
extends GridContainer

var webpack_pid: int = -1;

func get_pid_of_webpack_process() -> void:
	var output: Array[String] = [];
	var new_pid = -1;
	
	match OS.get_name():
		"Linux", "FreeBSD", "NetBSD", "OpenBSD", "BSD":
			if OS.execute("pidof",["webpack"],output) == -1:
				output = [];
			if output.size() == 1:
				output[0] = output[0].left(-1) # cut the right most char, which is \n 
			
			if output[0].is_valid_int() == false:
				webpack_pid = -1;
			else:
				new_pid =  output[0].to_int();
		_:
			print("Not implement for this platform!")
	webpack_pid = new_pid;



func update_button_availability()->void:
	get_pid_of_webpack_process();
	
	if(webpack_pid == -1):
		$"WebpackControlButtons/StartWebpack".disabled = false;
		$"WebpackControlButtons/StopWebpack".disabled = true;
	else:
		$"WebpackControlButtons/StartWebpack".disabled = true;
		$"WebpackControlButtons/StopWebpack".disabled = false;
		$CurrentPidBox/PidLabel.text = "PID:" + str(webpack_pid);

func stop_webpack_server() -> void:
	match OS.get_name():
		"Linux", "FreeBSD", "NetBSD", "OpenBSD", "BSD":
			if OS.execute("kill",[str(webpack_pid)]) == -1:
				print("failed to kill PID", webpack_pid);
			else:
				webpack_pid = -1;
		_:
			print("Not implement for this platform!")

func start_webpack_server() -> void:
	match OS.get_name():
		"Linux", "FreeBSD", "NetBSD", "OpenBSD", "BSD":
			print("start webpack");
			var resolvedPath = "";
			var pathString: String = $"webpack Path/pathTextField/pathText".text;
			if pathString.begins_with("res://"):
				pathString = ProjectSettings.globalize_path($"webpack Path/pathTextField/pathText".text);
			else: 
				pathString = pathString;
			var cmd = "cd "+ pathString+" && npm run start";
			cmd = "npm";
			var out = [];
			var res = OS.execute(cmd,["run","webpack"],out,true,true)
			if res == -1:
				print("Failed to execute webpack command",cmd)
			else:
				print("started webpack server",out)
	
		_:
			print("Not implement for this platform!")

	update_button_availability();

func _ready():
	match OS.get_name():
		"Linux", "FreeBSD", "NetBSD", "OpenBSD", "BSD":
			pass
		_:
			print("Not implement for this platform!")
	update_button_availability();


var accumulator:int = 0;
func _process(delta):
	accumulator += delta;
	if accumulator < 2:
		return;
	accumulator = 0;
	update_button_availability();
