versions.push_back({
    .GAME_VERSION             = "2026101300-010",

    .DLL_CODE_SIZE            = 0x00c64000, // optional header SizeOfCode
    .DLL_ENTRYPOINT           = 0x00b0294c, // optional header AddressOfEntryPoint
    .DLL_IMAGE_SIZE           = 0x0c5fe000, // optional header SizeOfImage

    // === 20261013 rebase off bm2dx.dll.091500 (=20260916 build). All code
    //     RVAs below were re-confirmed by byte-pattern matching against the
    //     new dll (unique multi-sample hits); data globals were re-resolved
    //     through their getter/setter/vtable-store sites. Unresolved entries
    //     stay nullptr and their hooks self-guard, as in 2026091600-010. ===
    .SOFT_REV_PATCH           = base + 0x009352e9, // call site bytes identical to 20260916 (delta +0x330)
    .INPUT_POLL_FN            = base + 0x00a676b0, // IO::InputManagerIIDX vftable slot 3 (vtable @ 0xd9f1f0)

    .ARENA_PHASE_PATCH        = base + 0x00593fa0, // stub returning local arena phase (multi-sample 3/7)
    .MUSIC_SELECT_CTOR        = base + 0x005f6a50, // builds mlist_sel/thumbnail textures (5/7)
    .DAN_SELECT_CTOR          = base + 0x008ba7b0, // CDanSelectScene scene enter (4/7)
    .SCENE_DTOR               = nullptr, // not resolved in 34; hooks stay guarded
    .STAGE_RESULT_FN          = base + 0x009143b0, // CStageResultScene scene enter
    .RESET_STATE_FN           = nullptr, // no confirmed equivalent
    .MSELECT_GENRE_C          = base + 0x005f3210, // renders hovered entry genre+title
    .MSELECT_GENRE_B          = base + 0x005f33d4, // cmp music_entry::texture_genre, 0
    .MSELECT_GENRE_A          = base + 0x005f34bd, // call drawing the genre text
    .MSELECT_GENRE_TEXT_FN    = base + 0x005dec80, // callee of MSELECT_GENRE_A (wide text renderer)
    .MSELECT_GENRE_ENTRY_OFFSET = 0x28,
    .INIT_TEXT_RENDER_FN      = base + 0x005deaf0, // boot debug ASCII text renderer wrapper (6/7)
    .BOOT_MODEL_TEXT_CALL     = base + 0x009272ad, // call drawing the boot model string (target 0x5dddc0)
    .TITLE_MODEL_TEXT_CALL    = base + 0x0091d9bc, // call drawing the title model string (target TEXT_RENDER_FN)

    .TEXT_INIT_FN             = base + 0x0032df90, // text property block initializer (7/7)
    .TEXT_RENDER_FN           = base + 0x005ddeb0, // ASCII text renderer wrapper (6/7)

    .RESULT_ARTIST_FN         = base + 0x009071a0, // result scene artist text draw
    .LOAD_CHART_FN_B          = base + 0x008141d0, // reads one .1 chart into the scratch buffer
    .LOAD_CHART_FN_A          = base + 0x008142f0, // outer loader (player-aware)
    .REMAP_INDEX_FN           = nullptr, // left null as in 2026091600
    .CHART_ANALYZE_FN         = base + 0x00813060, // boot chart load+analyze (prologue byte-identical)
    .CHART_ANALYZE_RESULT     = base + 0x00813287, // instruction after the chart fread (6/7)
    .LOAD_CHART_READ_RESULT   = base + 0x008142b8, // instruction after the play loader's final fread
    .CHART_CALC_RADAR_FN      = base + 0x00813b80, // radar calc for one difficulty

    .GET_APP_CONFIG           = base + 0x00922da0, // mov rax, cs:qword; retn (bytes match shape)
    .GET_MUSIC_DATA           = base + 0x00939430, // lea rax,[rip]; ret -> music_data blob
    .LOAD_AUDIO_FN            = base + 0x00a99be0, // BGM/sound player init
    .GET_SOUND_ENTRY_FN       = base + 0x00a99880, // singleton getter (lea rax,[rip]; ret)

    .XRPC_APPLY_FN            = nullptr, // unresolved in 34; stays null
    .REG_DISPATCH_FN          = nullptr,
    .REG_PATCH_ADDR           = nullptr,
    .SCORE_INVALID_FN         = nullptr,
    .DAN_SAVE_FN              = nullptr,
    .EAAPPLI_SAVE_FN          = nullptr,
    .MDATA_LOAD_FN            = base + 0x0093a230, // parses music_data blob post-load
    .IS_BTN_DOWN_FN_A         = base + 0x0080b19d, // cmp [rax+0x2A29FC],0 inside `button pressed`
    .IS_BTN_DOWN_FN_B         = base + 0x0080b23d, // same gate inside `button held`
    .AUTO_BEAM_PATCH          = base + 0x008f0f5b, // autoplay gate in lane beam renderer (bytes identical)
    .AUTO_BEAM_FN             = base + 0x008f0d00, // lane beam renderer
    .CARD_OUT_VFUNC           = nullptr, // vftable is in .data
    .RENDERER_PATCH           = base + 0x005b88cc, // call nop'd during vtable swap
    .APPLY_RANDOM_FN          = base + 0x0080fb80, // apply random modifier
    .DARK_MODE_PATCH          = nullptr, // no single site found in 34
    .MEASURE_PATCH            = base + 0x008f113b, // jz skipping measure-bar draw block (bytes identical)
    .BPM_BAR_PATCH            = nullptr,
    .BPM_BAR_PATCH_JMP        = 0,
    .PLAY_FIELD_LOAD          = base + 0x00808cf0, // play field note table fill (6/7)
    .RETRY_CHECK_A            = nullptr,
    .RETRY_CHECK_B            = nullptr,
    .GHOST_TARGET_FN          = nullptr,
    .GRAPH_TARGET_FN          = nullptr,
    .GRAPH_CONDITION          = nullptr,
    .FAIL_ANIMATION_FN        = nullptr,
    .FAIL_PLAY_SFX_FN         = nullptr,
    .FAIL_DURATION_JMP        = nullptr,

    .WNDPROC_FN               = base + 0x00925710, // PropClassWindowProc (prologue identical)

    .TIMING_HOOK_FN           = nullptr, // 34 has no pre-play timing dispatcher
    .ATTRACT_SELECT_FN        = base + 0x008b19a0, // picks next attract demo chart

    .GET_PLAY_OPTIONS_FN      = base + 0x00871aa0, // mov rax, cs:qword; retn
    .SPRITE_DRAW_FN           = base + 0x003349e0, // creates a named sprite (5/7)
    .JUDGE_APPLY_FN           = base + 0x0080d4a0, // applies a note judgment
    .JUDGE_DISPLAY_FN         = base + 0x008f4fd0, // updates judge/combo/fast-slow draw state (prologue identical)
    .JUDGE_DRAW_FS_KEYS_FN    = base + 0x008f47a0, // draws FAST/SLOW for keys
    .JUDGE_DRAW_FS_SC_FN      = base + 0x008f4a90, // draws separate scratch FAST/SLOW
    .JUDGE_DISPLAY_INIT_FN    = base + 0x008f4d60, // initialises one player's judgment display
    .JUDGE_PRESS_RETURN       = base + 0x0080e0d1, // return address of general timing calc
    .JUDGE_RELEASE_RETURN     = base + 0x0080d07a, // return address of CN release timing calc

    // offsets: data
    .GAME_MODEL               = base + 0x01040fc0, // "***:*:*:*:**********" mutable copy (string-confirmed)
    .GAME_STATE               = base + 0x0b60a5d0, // state_t blob: cross-checked via diff setter (+8) and active setter (+0x10)
    .INPUT_MANAGER            = base + 0x00bccd4b0, // InputManagerIIDX singleton (vtable store site confirmed; vtable @ 0xd9f1f0)
    .PLAYER_BLOCK             = base + 0x0b1276e0, // play engine global (getter @ 0x811010; call sites confirmed)
    .SCORES_P1                = nullptr, // 34 has no large static score tables
    .SCORES_P2                = nullptr,
    .RIVAL_SCORES_P1          = nullptr,
    .RIVAL_SCORES_P2          = nullptr,
    .MAX_ENTRIES              =        0x00080e8, // music entries per play style
    .D3D9_DEVICE              = base + 0x0b60b210, // [candidate] GAME_STATE+0xC40 (same delta as 20260916)
    .AUTO_PLAY                = base + 0x0b3ca0dc, // PLAYER_BLOCK+0x2A29FC (displacement verified in IS_BTN_DOWN)
    .RANDOM_DATA              = nullptr,
    .PLAY_STATE               = nullptr,
    .PLAY_SESSION             = base + 0x0b4dccc0, // session block (getter @ 0x8f0690; 34/37 call-site votes)
    .DEAD_STATE               = base + 0x0b4dc870  // per-player alive flags (getter @ 0x8e98f0; 45/45 votes)
});
