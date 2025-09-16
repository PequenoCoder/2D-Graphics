extends RigidBody2D

@export var min_speed = 150  # Minimum speed range.
@export var max_speed = 250  # Maximum speed range.
var mob_types = ["walk", "swim", "fly"]

func _ready():
	var mob_type = mob_types[randi() % mob_types.size()]
	$AnimatedSprite2D.animation = mob_type
	$AnimatedSprite2D.play()
