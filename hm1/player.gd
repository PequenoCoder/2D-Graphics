extends CharacterBody2D
signal hit

@export var speed = 400  # How fast the player will move (pixels/sec).
var screen_size  # Size of the game window.
var is_hit = false  # Flag to prevent multiple hits

func _ready():
	screen_size = get_viewport_rect().size
	hide()

func _process(delta):
	var velocity = Vector2()  # The player's movement vector.
	if Input.is_action_pressed("ui_right"):
		velocity.x += 1
	if Input.is_action_pressed("ui_left"):
		velocity.x -= 1
	if Input.is_action_pressed("ui_down"):
		velocity.y += 1
	if Input.is_action_pressed("ui_up"):
		velocity.y -= 1
	if velocity.length() > 0:
		velocity = velocity.normalized() * speed
		$AnimatedSprite2D.play()
	else:
		$AnimatedSprite2D.stop()
		
	velocity *= delta
	var collision = move_and_collide(velocity)
	if collision:
		# Player hit a wall, stop movement
		pass
	position.x = clamp(position.x, 0, screen_size.x)
	position.y = clamp(position.y, 0, screen_size.y)

	if velocity.x != 0:
		$AnimatedSprite2D.animation = "right"
		$AnimatedSprite2D.flip_v = false
		# See the note below about boolean assignment
		$AnimatedSprite2D.flip_h = velocity.x < 0
	elif velocity.y != 0:
		$AnimatedSprite2D.animation = "up"
		$AnimatedSprite2D.flip_v = velocity.y > 0


func _on_Player_body_entered(_body):
	if is_hit:
		return
		
	is_hit = true
	
	set_physics_process(false)
	
	create_fade_effect()
	
	#Hide the original player sprite immediately
	$AnimatedSprite2D.visible = false
	
	emit_signal("hit")
	
	#Hide the entire player after fade completes
	await get_tree().create_timer(3.0).timeout
	$CollisionShape2D.set_deferred("disabled", true)
	hide()

func create_fade_effect():
	var fade_sprite = FadeSprite.new()
	
	var current_texture = $AnimatedSprite2D.sprite_frames.get_frame_texture($AnimatedSprite2D.animation, $AnimatedSprite2D.frame)
	fade_sprite.set_texture(current_texture)
	
	fade_sprite.scale = $AnimatedSprite2D.scale
	fade_sprite.flip_h = $AnimatedSprite2D.flip_h
	fade_sprite.flip_v = $AnimatedSprite2D.flip_v
	
	fade_sprite.fade_duration = 3.0
	
	get_parent().add_child(fade_sprite)
	fade_sprite.global_position = $AnimatedSprite2D.global_position
	
	fade_sprite.start_fade_out()
	
	fade_sprite.connect("fade_completed", func(node): node.queue_free())

func start(pos):
	position = pos
	show()
	$CollisionShape2D.disabled = false
	$AnimatedSprite2D.visible = true
	set_physics_process(true)
	is_hit = false


func _on_visible_on_screen_notifier_2d_screen_exited() -> void:
	queue_free()


func _on_end_zone_area_entered(area: Area2D) -> void:
	pass # Replace with function body.
