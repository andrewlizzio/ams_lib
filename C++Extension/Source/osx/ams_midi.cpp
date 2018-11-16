/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_midi.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Variables
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

AUGraph* AMS::MIDI::au_graph = nullptr;
AudioUnit* AMS::MIDI::au_synth = nullptr;


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

bool AMS::MIDI::c_is_device_valid() {
    if (au_graph != nullptr) {
        Boolean state;
        AUGraphIsOpen(*au_graph, &state);
        return state ? true : false;
    }
    else
        return false;
}

bool AMS::MIDI::c_out_message(unsigned char arg0, unsigned char arg1, unsigned char arg2) {
    if (c_is_device_valid()) {
        OSStatus status = MusicDeviceMIDIEvent(*au_synth, arg0, arg1, arg2, 0);
        return status == 0 ? true : false;
    }
    else
        return false;
}

bool AMS::MIDI::c_edit_channel_controller(unsigned char channel, unsigned char cnum, unsigned char cval) {
    return c_out_message(0xB0 | channel, cnum, cval);
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::MIDI::rbf_get_device_handle(VALUE self) {
    return RU::to_value((long long)au_graph);
}

VALUE AMS::MIDI::rbf_is_device_open(VALUE self) {
    return RU::to_value(c_is_device_valid());
}

VALUE AMS::MIDI::rbf_open_device(VALUE self) {
    if (au_graph != nullptr) {
        Boolean state;
        AUGraphIsOpen(*au_graph, &state);
        if (state)
            return Qfalse;
        else {
            DisposeAUGraph(*au_graph);
            delete au_graph;
            au_graph = nullptr;
            if (au_synth != nullptr) {
                delete au_synth;
                au_synth = nullptr;
            }
        }
    }
    AUNode synth_node;
    AUNode out_node;
    AudioComponentDescription cd;
    OSStatus status;
    // Create Graph
    au_graph = new AUGraph;
    status = NewAUGraph(au_graph);
    if (status != 0) {
        delete au_graph;
        au_graph = nullptr;
        return Qfalse;
    }
    // Set component description
    cd.componentManufacturer = 'appl';
    cd.componentFlags = 0;
    cd.componentFlagsMask = 0;
    // Add Synth Node
    cd.componentType = 'aumu';
    cd.componentSubType = 'dls ';
    status = AUGraphAddNode(*au_graph, &cd, &synth_node);
    if (status != 0) {
        DisposeAUGraph(*au_graph);
        delete au_graph;
        au_graph = nullptr;
        return Qfalse;
    }
    // Add Out Node
    cd.componentType = 'auou';
    cd.componentSubType = 'def ';
    status = AUGraphAddNode(*au_graph, &cd, &out_node);
    if (status != 0) {
        DisposeAUGraph(*au_graph);
        delete au_graph;
        au_graph = nullptr;
        return Qfalse;
    }
    // Open Graph
    status = AUGraphOpen(*au_graph);
    if (status != 0) {
        DisposeAUGraph(*au_graph);
        delete au_graph;
        au_graph = nullptr;
        return Qfalse;
    }
    // Connect Synth to Out
    status = AUGraphConnectNodeInput(*au_graph, synth_node, 0, out_node, 0);
    if (status != 0) {
        AUGraphClose(*au_graph);
        DisposeAUGraph(*au_graph);
        delete au_graph;
        au_graph = nullptr;
        return Qfalse;
    }
    // Graph info
    au_synth = new AudioUnit;
    status = AUGraphNodeInfo(*au_graph, synth_node, nullptr, au_synth);
    if (status != 0) {
        AUGraphClose(*au_graph);
        DisposeAUGraph(*au_graph);
        delete au_graph;
        delete au_synth;
        au_graph = nullptr;
        au_synth = nullptr;
        return Qfalse;
    }
    // Init graph
    status = AUGraphInitialize(*au_graph);
    if (status != 0) {
        AUGraphClose(*au_graph);
        DisposeAUGraph(*au_graph);
        delete au_graph;
        delete au_synth;
        au_graph = nullptr;
        au_synth = nullptr;
        return Qfalse;
    }
    // Start graph
    status = AUGraphStart(*au_graph);
    if (status != 0) {
        AUGraphClose(*au_graph);
        DisposeAUGraph(*au_graph);
        delete au_graph;
        delete au_synth;
        au_graph = nullptr;
        au_synth = nullptr;
        return Qfalse;
    }
    else
        return Qtrue;
}

VALUE AMS::MIDI::rbf_close_device(VALUE self) {
    if (au_graph != nullptr) {
        AUGraphStop(*au_graph);
        AUGraphClose(*au_graph);
        DisposeAUGraph(*au_graph);
        delete au_graph;
        delete au_synth;
        au_graph = nullptr;
        au_synth = nullptr;
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::MIDI::rbf_out_message(int argc, VALUE* argv, VALUE self) {
    unsigned char arg0, arg1, arg2;
    if (argc == 3) {
        arg0 = RU::value_to_uchar(argv[0]);
        arg1 = RU::value_to_uchar(argv[1]);
        arg2 = RU::value_to_uchar(argv[2]);
    }
    else if (argc == 2) {
        arg0 = RU::value_to_uchar(argv[0]);
        arg1 = RU::value_to_uchar(argv[1]);
        arg2 = 0;
    }
    else if (argc == 1) {
        arg0 = RU::value_to_uchar(argv[0]);
        arg1 = 0;
        arg2 = 0;
    }
    else {
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    }
    return RU::to_value(c_out_message(arg0, arg1, arg2));
}

VALUE AMS::MIDI::rbf_get_volume(VALUE self) {
    return rb_ary_new3(2, RU::to_value(1.0), RU::to_value(1.0));
}

VALUE AMS::MIDI::rbf_set_volume(int argc, VALUE* argv, VALUE self) {
    if (!c_is_device_valid()) return Qfalse;
    unsigned int left, right;
    if (argc == 2) {
        left = static_cast<unsigned int>(Geom::clamp_double(RU::value_to_double(argv[0]), 0.0, 1.0) * 127.0);
        right = static_cast<unsigned int>(Geom::clamp_double(RU::value_to_double(argv[1]), 0.0, 1.0) * 127.0);
    }
    else if (argc == 1) {
        unsigned int vol = static_cast<unsigned int>(Geom::clamp_double(RU::value_to_double(argv[0]), 0.0, 1.0) * 127.0);
        left = vol;
        right = vol;
    }
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    c_out_message(0xB0, 0x07, left > right ? left : right);
    c_out_message(0xB0, 0x08, Geom::clamp<unsigned char>(63 - left / 2 + right / 2, 0, 127));
    return Qtrue;
}

VALUE AMS::MIDI::rbf_reset(VALUE self) {
    return RU::to_value(c_out_message(0xB0, 0x79, 0));
}

VALUE AMS::MIDI::rbf_play_note(int argc, VALUE* argv, VALUE self) {
    if (!c_is_device_valid()) return Qnil;
    unsigned char instrument, note, channel, volume;
    channel = 0;
    volume = 127;
    if (argc == 4) {
        channel = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[2]), 0, 15);
        volume = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[3]), 0, 127);
    }
    else if (argc == 3) {
        channel = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[2]), 0, 15);
    }
    else if (argc != 2) {
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..4 arguments.");
    }
    instrument = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[0]), 0, 127);
    note = Geom::clamp<unsigned char>(RU::value_to_uchar(argv[1]), 0, 127);
    // Set instrument
    c_out_message(0xC0 | channel, instrument, 0);
    // Play note
    c_out_message(0x90 | channel, note, volume);
    // Return nil if no success
    unsigned long id = instrument << 16 | (note << 8 | channel);
    return RU::to_value(id);
}

VALUE AMS::MIDI::rbf_stop_note(VALUE self, VALUE v_id) {
    if (!c_is_device_valid()) return Qfalse;
    unsigned long id = RU::value_to_ulong(v_id);
    unsigned char instrument = Geom::clamp<unsigned char>((unsigned char)(id >> 16), 0, 127);
    unsigned char note = Geom::clamp<unsigned char>((id & 0xFFFF) >> 8, 0, 127);
    unsigned char channel = Geom::clamp<unsigned char>((id & 0xFFFF) & 0xFF, 0, 15);
    // Set instrument
    c_out_message(0xC0 | channel, instrument, 0);
    // Stop note
    c_out_message(0x80 | channel, note, 0);
    // Return success
    return Qtrue;
}

VALUE AMS::MIDI::rbf_change_channel_controller(VALUE self, VALUE v_channel, VALUE v_cnum, VALUE v_cval) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char cnum = Geom::clamp<unsigned char>(RU::value_to_uchar(v_cnum), 0, 127);
    unsigned char cval = Geom::clamp<unsigned char>(RU::value_to_uchar(v_cval), 0, 127);
    return RU::to_value(c_edit_channel_controller(channel, cnum, cval));
}

VALUE AMS::MIDI::rbf_change_channel_volume(VALUE self, VALUE v_channel, VALUE v_volume) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char volume = Geom::clamp<unsigned char>(RU::value_to_uchar(v_volume), 0, 127);
    return RU::to_value(c_edit_channel_controller(channel, 0x07, volume));
}

VALUE AMS::MIDI::rbf_change_channel_stereo_pan(VALUE self, VALUE v_channel, VALUE v_pan) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char pan = Geom::clamp<unsigned char>(RU::value_to_uchar(v_pan), 0, 127);
    return RU::to_value(c_edit_channel_controller(channel, 0x0A, pan));
}

VALUE AMS::MIDI::rbf_change_channel_expression(VALUE self, VALUE v_channel, VALUE v_expression) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char expression = Geom::clamp<unsigned char>(RU::value_to_uchar(v_expression), 0, 127);
    return RU::to_value(c_edit_channel_controller(channel, 0x0B, expression));
}

VALUE AMS::MIDI::rbf_sustain_channel_pedal(VALUE self, VALUE v_channel, VALUE v_state) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    return RU::to_value(c_edit_channel_controller(channel, 0x40, RTEST(v_state) ? 127 : 0));
}

VALUE AMS::MIDI::rbf_reset_channel_controllers(VALUE self, VALUE v_channel) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    return RU::to_value(c_edit_channel_controller(channel, 0x79, 0));
}

VALUE AMS::MIDI::rbf_set_note_position(VALUE self, VALUE v_id, VALUE v_position, VALUE v_max_hearing_range) {
    if (!c_is_device_valid()) return Qfalse;
    unsigned long id = RU::value_to_ulong(v_id);
    unsigned char instrument = Geom::clamp<unsigned char>((unsigned char)(id >> 16), 0, 127);
    //~ unsigned char note = Geom::clamp<unsigned char>((id & 0xFFFF) >> 8, 0, 127);
    unsigned char channel = Geom::clamp<unsigned char>((id & 0xFFFF) & 0xFF, 0, 15);
    Geom::Vector3d position, eye, xaxis, yaxis, zaxis;
    RU::value_to_vector(v_position, position);
    double max_hearing_range = Geom::max_double(RU::value_to_double(v_max_hearing_range), 1.0);
    // Set instrument
    c_out_message(0xC0 | channel, instrument, 0);
    // Get camera matrix
    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    VALUE v_cam = rb_funcall(v_view, RU::INTERN_CAMERA, 0);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_EYE, 0), eye);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_XAXIS, 0), xaxis);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_YAXIS, 0), yaxis);
    RU::value_to_vector(rb_funcall(v_cam, RU::INTERN_ZAXIS, 0), zaxis);
    Geom::Transformation matrix(zaxis, xaxis, yaxis, eye);
    matrix.m_xaxis.m_w = 0.0;
    matrix.m_yaxis.m_w = 0.0;
    matrix.m_zaxis.m_w = 0.0;
    matrix.m_origin.m_w = 1.0;
    Geom::Transformation matrix_inv(matrix.inverse());
    // Transform position relative to camera matrix
    Geom::Vector3d pt(matrix_inv.transform_vector(position));
    // Calculate volume
    double volume = (1.0 - Geom::clamp_double(pt.get_length() / max_hearing_range, 0.0, 1.0)) * 127.0;
    unsigned char m_volume = static_cast<unsigned char>(volume + 0.5);
    c_edit_channel_controller(channel, 0x07, m_volume);
    // Calculate stereo pan
    double dist_sq = pt.m_x * pt.m_x + pt.m_y * pt.m_y;
    double ratio;
    if (dist_sq > M_EPSILON_SQ)
        ratio = pt.m_y / dist_sq * 63.5 + 63.5;
    else
        ratio = 63.5;
    unsigned char bratio = static_cast<unsigned char>(ratio + 0.5);
    c_edit_channel_controller(channel, 0x0A, bratio);
    // Return success
    return Qtrue;
}

VALUE AMS::MIDI::rbf_change_channel_instrument(VALUE self, VALUE v_channel, VALUE v_instrument) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char instrument = Geom::clamp<unsigned char>(RU::value_to_uchar(v_instrument), 0, 127);
    return RU::to_value(c_out_message(0xC0 | channel, instrument, 0));
}

VALUE AMS::MIDI::rbf_play_note2(VALUE self, VALUE v_channel, VALUE v_note, VALUE v_volume) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char note = Geom::clamp<unsigned char>(RU::value_to_uchar(v_note), 0, 127);
    unsigned char volume = Geom::clamp<unsigned char>(RU::value_to_uchar(v_volume), 0, 127);
    return RU::to_value(c_out_message(0x90 | channel, note, volume));
}

VALUE AMS::MIDI::rbf_stop_note2(VALUE self, VALUE v_channel, VALUE v_note, VALUE v_volume) {
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    unsigned char note = Geom::clamp<unsigned char>(RU::value_to_uchar(v_note), 0, 127);
    unsigned char volume = Geom::clamp<unsigned char>(RU::value_to_uchar(v_volume), 0, 127);
    return RU::to_value(c_out_message(0x80 | channel, note, volume));
}

VALUE AMS::MIDI::rbf_change_channel_position(VALUE self, VALUE v_channel, VALUE v_position, VALUE v_max_hearing_range) {
    if (!c_is_device_valid()) return Qfalse;
    unsigned char channel = Geom::clamp<unsigned char>(RU::value_to_uchar(v_channel), 0, 15);
    Geom::Vector3d position, eye, xaxis, yaxis, zaxis;
    RU::value_to_vector(v_position, position);
    double max_hearing_range = Geom::max_double(RU::value_to_double(v_max_hearing_range), 1.0);
    // Get camera matrix
    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    VALUE v_cam = rb_funcall(v_view, RU::INTERN_CAMERA, 0);
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_EYE, 0), eye );
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_XAXIS, 0), xaxis );
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_YAXIS, 0), yaxis );
    RU::value_to_vector( rb_funcall(v_cam, RU::INTERN_ZAXIS, 0), zaxis );
    Geom::Transformation matrix(zaxis, xaxis, yaxis, eye);
    matrix.m_xaxis.m_w = 0.0;
    matrix.m_yaxis.m_w = 0.0;
    matrix.m_zaxis.m_w = 0.0;
    matrix.m_origin.m_w = 1.0;
    Geom::Transformation matrix_inv(matrix.inverse());
    // Transform position relative to camera matrix
    Geom::Vector3d pt(matrix_inv.transform_vector(position));
    // Calculate volume
    double volume = (1.0 - Geom::clamp_double(pt.get_length() / max_hearing_range, 0.0, 1.0)) * 127.0;
    unsigned char bvolume = static_cast<unsigned char>(volume + 0.5);
    c_edit_channel_controller(channel, 0x07, bvolume);
    // Calculate stereo pan
    double dist_sq = pt.m_x * pt.m_x + pt.m_y * pt.m_y;
    double ratio;
    if (dist_sq > M_EPSILON_SQ)
        ratio = pt.m_y / dist_sq * 63.5 + 63.5;
    else
        ratio = 63.5;
    unsigned char bratio = static_cast<unsigned char>(ratio + 0.5);
    c_edit_channel_controller(channel, 0x0A, bratio);
    // Return success
    return Qtrue;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::MIDI::init_ruby(VALUE mAMS) {
    VALUE mMIDI = rb_define_module_under(mAMS, "MIDI");

    rb_define_module_function(mMIDI, "get_device_handle", VALUEFUNC(AMS::MIDI::rbf_get_device_handle), 0);
    rb_define_module_function(mMIDI, "is_device_open?", VALUEFUNC(AMS::MIDI::rbf_is_device_open), 0);
    rb_define_module_function(mMIDI, "open_device", VALUEFUNC(AMS::MIDI::rbf_open_device), 0);
    rb_define_module_function(mMIDI, "close_device", VALUEFUNC(AMS::MIDI::rbf_close_device), 0);
    rb_define_module_function(mMIDI, "out_message", VALUEFUNC(AMS::MIDI::rbf_out_message), -1);
    rb_define_module_function(mMIDI, "get_volume", VALUEFUNC(AMS::MIDI::rbf_get_volume), 0);
    rb_define_module_function(mMIDI, "set_volume", VALUEFUNC(AMS::MIDI::rbf_set_volume), -1);
    rb_define_module_function(mMIDI, "reset", VALUEFUNC(AMS::MIDI::rbf_reset), 0);
    rb_define_module_function(mMIDI, "play_note", VALUEFUNC(AMS::MIDI::rbf_play_note), -1);
    rb_define_module_function(mMIDI, "stop_note", VALUEFUNC(AMS::MIDI::rbf_stop_note), 1);
    rb_define_module_function(mMIDI, "change_channel_controller", VALUEFUNC(AMS::MIDI::rbf_change_channel_controller), 3);
    rb_define_module_function(mMIDI, "change_channel_volume", VALUEFUNC(AMS::MIDI::rbf_change_channel_volume), 2);
    rb_define_module_function(mMIDI, "change_channel_stereo_pan", VALUEFUNC(AMS::MIDI::rbf_change_channel_stereo_pan), 2);
    rb_define_module_function(mMIDI, "change_channel_expression", VALUEFUNC(AMS::MIDI::rbf_change_channel_expression), 2);
    rb_define_module_function(mMIDI, "sustain_channel_pedal", VALUEFUNC(AMS::MIDI::rbf_sustain_channel_pedal), 2);
    rb_define_module_function(mMIDI, "reset_channel_controllers", VALUEFUNC(AMS::MIDI::rbf_reset_channel_controllers), 1);
    rb_define_module_function(mMIDI, "set_note_position", VALUEFUNC(AMS::MIDI::rbf_set_note_position), 3);
    rb_define_module_function(mMIDI, "change_channel_instrument", VALUEFUNC(AMS::MIDI::rbf_change_channel_instrument), 2);
    rb_define_module_function(mMIDI, "play_note2", VALUEFUNC(AMS::MIDI::rbf_play_note2), 3);
    rb_define_module_function(mMIDI, "stop_note2", VALUEFUNC(AMS::MIDI::rbf_stop_note2), 3);
    rb_define_module_function(mMIDI, "change_channel_position", VALUEFUNC(AMS::MIDI::rbf_change_channel_position), 3);
}
