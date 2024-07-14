extends Node2D


@export var Ball : PackedScene = preload("res://objects/Ball.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


var last_time_sent = 0
func _process(delta):

	if(last_time_sent < 0.1):
		last_time_sent += delta;
		return;
	
	last_time_sent =0;
	var balls = get_tree().get_nodes_in_group("ball")
	var dict = {};
	
	for i in range(balls.size()):
		var ball = balls[i];
		dict[i] = {
			"x": roundf(ball.get_position().x),
			"y": roundf(ball.get_position().y),
			"colorHex": '#'+ball.get_color().to_html()
		}	

	$WuiTab.send_event("BallInfo",dict);





func _unhandled_input(event):
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_RIGHT and event.pressed:
			print("Left button was clicked at ", event.position)
			
			var b = Ball.instantiate()
			(b as RigidBody2D).linear_velocity = Vector2(randf(),randf())*4000;
			b.position = event.position;
			get_tree().root.get_child(0).add_child(b);


