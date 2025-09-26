#include "fadesprite.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/timer.hpp>

using namespace godot;

void FadeSprite::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_fade_duration"), &FadeSprite::get_fade_duration);
	ClassDB::bind_method(D_METHOD("set_fade_duration", "p_duration"), &FadeSprite::set_fade_duration);
	ClassDB::bind_method(D_METHOD("get_is_fading"), &FadeSprite::get_is_fading);
	ClassDB::bind_method(D_METHOD("start_fade_out"), &FadeSprite::start_fade_out);
	ClassDB::bind_method(D_METHOD("start_fade_in"), &FadeSprite::start_fade_in);
	ClassDB::bind_method(D_METHOD("reset_visibility"), &FadeSprite::reset_visibility);
	ClassDB::bind_method(D_METHOD("_on_fade_completed"), &FadeSprite::_on_fade_completed);
	ClassDB::bind_method(D_METHOD("_on_timer_timeout"), &FadeSprite::_on_timer_timeout);
	ClassDB::bind_method(D_METHOD("_on_fade_step"), &FadeSprite::_on_fade_step);
	
	//show up in the editor
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fade_duration", PROPERTY_HINT_RANGE, "0.1,5.0,0.1"), "set_fade_duration", "get_fade_duration");
	
	ADD_SIGNAL(MethodInfo("fade_started", PropertyInfo(Variant::OBJECT, "node")));
	ADD_SIGNAL(MethodInfo("fade_completed", PropertyInfo(Variant::OBJECT, "node")));
}

FadeSprite::FadeSprite() {
	fade_duration = 1.0;
	is_fading = false;
	is_visible_state = true;
	fade_timer = nullptr;
	fade_step_timer = nullptr;
	fade_start_alpha = 1.0;
	fade_target_alpha = 0.0;
	fade_current_alpha = 1.0;
}

FadeSprite::~FadeSprite() {
}

void FadeSprite::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;  
	}
	
	//create timers for fade effect
	fade_timer = memnew(Timer);
	fade_timer->set_one_shot(true); //run once
	fade_timer->connect("timeout", Callable(this, "_on_timer_timeout"));
	add_child(cast_to<Node>(fade_timer));
	
	//runs every frame to update the fade
	fade_step_timer = memnew(Timer);
	fade_step_timer->set_one_shot(false);
	fade_step_timer->set_wait_time(0.016); //about 60fps
	fade_step_timer->connect("timeout", Callable(this, "_on_fade_step"));
	add_child(cast_to<Node>(fade_step_timer));
}


void FadeSprite::set_fade_duration(const double p_duration) {
	fade_duration = p_duration;//set how long the fade takes
}

double FadeSprite::get_fade_duration() const {
	return fade_duration; //get the duration
}

bool FadeSprite::get_is_fading() const {
	return is_fading;//check if currently fading
}

void FadeSprite::start_fade_out() {
	if (is_fading || !is_visible_state) {
		return; //already fading or not visible
	}
	
	is_fading = true;
	is_visible_state = false;
	
	emit_signal("fade_started", this);//tell other nodes we started fading
	
	//start with full
	set_modulate(Color(1, 1, 1, 1.0));
	
	if (fade_timer) {
		fade_timer->set_wait_time(fade_duration);
		fade_timer->start();
	}
	
	start_gradual_fade(1.0, 0.0); //fade
}

void FadeSprite::start_fade_in() {
	if (is_fading || is_visible_state) {
		return; //already fading 
	}
	
	is_fading = true;
	is_visible_state = true;
	
	emit_signal("fade_started", this);
	
	//start with transparent
	set_modulate(Color(1, 1, 1, 0.0));
	
	if (fade_timer) {
		fade_timer->set_wait_time(fade_duration);
		fade_timer->start();
	}
	
	//fade in
	start_gradual_fade(0.0, 1.0);
}

void FadeSprite::reset_visibility() {
	if (fade_timer) {
		fade_timer->stop();//stop the timer
	}
	
	is_fading = false;
	is_visible_state = true;
	set_modulate(Color(1, 1, 1, 1));//make it fully visible again
}


void FadeSprite::_on_fade_completed() {
	is_fading = false;
	emit_signal("fade_completed", this);//tell other nodes we're done fading
}

void FadeSprite::_on_timer_timeout() {
	_on_fade_completed();//when timer runs out, fade is complete
}

void FadeSprite::start_gradual_fade(double from_alpha, double to_alpha) {
	fade_start_alpha = from_alpha;
	fade_target_alpha = to_alpha;
	fade_current_alpha = from_alpha;
	
	//stop any existing fade
	if (fade_step_timer) {
		fade_step_timer->stop();
	}
	
	//start the step timer to update gradually
	if (fade_step_timer) {
		fade_step_timer->start();
	}
}

void FadeSprite::_on_fade_step() {
	//calculate time
	double elapsed_time = 0.0;
	if (fade_timer) {
		elapsed_time = fade_duration - fade_timer->get_time_left();
	}
	double progress = elapsed_time / fade_duration;
	
	if (progress < 0.0) progress = 0.0;
	if (progress > 1.0) progress = 1.0;
	
	fade_current_alpha = fade_start_alpha + (fade_target_alpha - fade_start_alpha) * progress;
	
	//apply the alpha
	set_modulate(Color(1, 1, 1, fade_current_alpha));
	
	//stop when we reach the end
	if (progress >= 1.0) {
		if (fade_step_timer) {
			fade_step_timer->stop();
		}
	}
}
