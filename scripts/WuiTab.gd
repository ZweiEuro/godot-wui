extends WuiTab

func _on_load_end(currentUrl, httpCode, error):
	print("signal inside godot on load url:",currentUrl," code: ", httpCode," error: ", error)


func _on_event_received(eventName, eventPayload):
	# print("wui event received ", eventName, " payload ",eventPayload)
	
	if(eventName == "DeleteBall"):
		var id = eventPayload["id"];
		var balls = get_tree().get_nodes_in_group("ball")
		if(balls.size() > 0 && balls[id]):
			balls[id].queue_free();
