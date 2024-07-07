extends RigidBody2D



# Called when the node enters the scene tree for the first time.
func _ready():
	$CollisionShape2D/ColorRect.set_color( Color(randf(),randf(),randf(),1))
	
func get_color():
	return 	$CollisionShape2D/ColorRect.get_color();
