versions.push_back({
    .GAME_VERSION             = "2026091600-010",

    .DLL_CODE_SIZE            = 0x00c64200, // optional header SizeOfCode
    .DLL_ENTRYPOINT           = 0x00b02b3c, // optional header AddressOfEntryPoint
    .DLL_IMAGE_SIZE           = 0x0c5ff000, // optional header SizeOfImage

    // === 34 offset port (20260916). Confirmed-by-disasm offsets are filled
    //     in; anything not yet resolved stays nullptr and its hook guards. ===
    .SOFT_REV_PATCH           = base + 0x00934fb9, // call replaced by `mov byte ptr [rdi+5], rev` (call sub_180958680, preceded by movzx ecx,[rdi+5])
    .INPUT_POLL_FN            = base + 0x00a67d70, // IO::InputManagerIIDX vftable slot 3 (no code callers; vftable @ 0x00d9ff20)

    .ARENA_PHASE_PATCH        = base + 0x005943d0, // stub: returns local arena phase (call sub_1805B6770 arena check -> return 1; else per-player phase from 0x18107C0F0 table). Ref 0x5943d0 matches byte-for-byte.
    .MUSIC_SELECT_CTOR        = base + 0x005f6e40, // builds mlist_sel/mlist_sel_bg/thumbnail/textures; scene enter for music select
    .DAN_SELECT_CTOR          = base + 0x008ba420, // CDanSelectScene vftable slot 13 (scene enter)
    .SCENE_DTOR               = nullptr, // not found in 34 (0x982760 area is list draw helpers in this build)
    .STAGE_RESULT_FN          = base + 0x00914080, // CStageResultScene scene enter (vtable forwarder 0x8BE970 -> jmp 0x914080); large init, reads active players via 0x92FD60
    .RESET_STATE_FN           = nullptr, // no confirmed equivalent
    .MSELECT_GENRE_C          = base + 0x005f3600, // renders the hovered entry's genre and title
    .MSELECT_GENRE_B          = base + 0x005f37c4, // cmp music_entry::texture_genre, 0 (7 bytes) @ [rax+3C8h], rax=music(=a1+28h)
    .MSELECT_GENRE_A          = base + 0x005f38ad, // call inside the above that draws the genre text (6th arg=[rsp+28h]=music+140h)
    .MSELECT_GENRE_TEXT_FN    = base + 0x005df070, // callee of MSELECT_GENRE_A (wide/UTF-16 text renderer)
    .MSELECT_GENRE_ENTRY_OFFSET = 0x28,
    .INIT_TEXT_RENDER_FN      = base + 0x005deee0, // boot debug ASCII text renderer wrapper (body calls text core 0x331880); ref 0x5e0250
    .BOOT_MODEL_TEXT_CALL     = base + 0x00926f7d, // call render boot model string (call 0x5DE1B0; ecx=1 centered, edx=3C0, r8d=21C, r9=0x18B4E34C8 text buffer)
    .TITLE_MODEL_TEXT_CALL    = base + 0x0091d68c, // call ASCII renderer 0x5DE2A0 for title card (artist string 0x18B4E28A0)

    .TEXT_INIT_FN             = base + 0x0032e480, // text property block initializer: sub_18032E480, first callee of 34 TEXT_RENDER_FN (matches init_text_t)
    .TEXT_RENDER_FN           = base + 0x005de2a0, // ASCII text renderer wrapper: sub_1805DE2A0(font,x,y,layer,props,text) -> core 0x331880; 32 rva 0x5e05d0 is a std::wstring helper in 34

    .RESULT_ARTIST_FN         = base + 0x00906e70, // result scene artist text draw: calls wide renderer 0x5DF070 with rcx=[r15+1C0h] (music artist wchar[128]) @ 0x9071E1
    .LOAD_CHART_FN_B          = base + 0x008141b0, // loader: (out, filename, chart_index) -> bool; memset 0x18000 scratch then read .1 file; sole caller 0x8142D0
    .LOAD_CHART_FN_A          = base + 0x008142d0, // outer: (out, player, filename, chart_index) -> bool; calls LOAD_CHART_FN_B
    .REMAP_INDEX_FN           = nullptr, // TODO: 0x814330 is int(int) switch (0->3,1->1,2->0,3->2...) used inside loader for file offset; signature mismatch with charts. left null + guarded
    .CHART_ANALYZE_FN         = base + 0x00813040, // boot chart load+analyze: (scratch, entry, v22, flag); memset 0x18000 then per-diff loop (stride 0x230); sole caller 0x93A006 (MDATA_LOAD_FN)
    .CHART_ANALYZE_RESULT     = base + 0x00813267, // instruction after the chart fread (call [0x180C66EB0] @ 0x813261, rdx=rdi=scratch, r8d=size); rax=bytes read, rdi=buffer
    .LOAD_CHART_READ_RESULT   = base + 0x00814298, // LOAD_CHART_FN_B+0xE8: instruction after the final fread (call [0x180C66EB0] @ 0x814292, rdx=rdi=output, r8d=[rsp+20h]); rax=bytes read
    .CHART_CALC_RADAR_FN      = base + 0x00813b60, // radar calc (int* rec, void* out) -> ptr to 24 bytes; = LOAD_CHART_FN_B - 0x650 (matches ref delta); callers 0x831711/0x836B2D/0x93A07B

    .GET_APP_CONFIG           = base + 0x00922a70, // mov rax, cs:qword_18b4e3360; retn

    .GET_MUSIC_DATA           = base + 0x00939100, // lea rax,[rip+0xACD3769]; ret -> music_data_t blob (B60C870). index table: 0x10 + occupied*4; entries stride 0x7F8, count [blob+8]
    .LOAD_AUDIO_FN            = base + 0x00a9a290, // BGM/sound player init: two stream sub-objects [+58h]/[+60h] (vftable 0xDF0688/0xDF06A8), PCM buffer [+170h]/[+178h]; ref 0xa9a2f0
    .GET_SOUND_ENTRY_FN       = base + 0x00a99f30, // singleton getter for global sound-player object (lea rax,[0x1810709E0]; ret); by-id variant 0xA99FC0
    .XRPC_APPLY_FN            = nullptr, // unresolved: 0xAB4000-0xABB7xx is a giant eamuse profile serializer; wrapper not pinned
    .REG_DISPATCH_FN          = nullptr,
    .REG_PATCH_ADDR           = nullptr,
    .SCORE_INVALID_FN         = nullptr,
    .DAN_SAVE_FN              = nullptr,
    .EAAPPLI_SAVE_FN          = nullptr,
    .MDATA_LOAD_FN            = base + 0x00939f00, // parses music_data blob after /data/info/0/music_data.bin is loaded (load=0x939D50, boot chain 0x927861)
    .IS_BTN_DOWN_FN_A         = base + 0x0080b17d, // cmp byte ptr [rax+0x2A29FC],0 inside the `button pressed` helper (0xA0 before B, matches 2025082500)
    .IS_BTN_DOWN_FN_B         = base + 0x0080b21d, // cmp byte ptr [rax+0x2A29FC],0 inside the `button held` helper
    .AUTO_BEAM_PATCH          = base + 0x008f0b7b, // cmp byte ptr [rax+0x2A29FC],0 in the lane beam renderer (0x25B after AUTO_BEAM_FN, matches 2025082500)
    .AUTO_BEAM_FN             = base + 0x008f0920, // lane beam renderer; rcx -> per-player block whose first dword is the player index
    .CARD_OUT_VFUNC           = nullptr, // CCardOut vftable is in .data - not in this .text-only dump
    .RENDERER_PATCH           = base + 0x005b8cdc, // call 0x5D5790 draw loader/boot frame (rcx=0xB0EA060 global, rdx=frame struct, r8=0xC723A0) - nop'd to skip; ref 0x5b8d12
    .APPLY_RANDOM_FN          = base + 0x0080fb60, // apply random modifier: full frame, per-player check, tail 0x80FDB1-0x80FE51 = 8x call 0x80EBE0 (lane order); helpers 0x80E9A0/0x80F940/0x80F9F0/0x80EA40/0x80ECD0
    .DARK_MODE_PATCH          = nullptr, // no single je-over-playfield-draw site found in 0x8F0-0x8F3; candidate enable/disable switch [rax+28h] @ 0x8F0AEA
    .MEASURE_PATCH            = base + 0x008f0d5b, // jz 0x8F0E8D skipping whole measure-bar draw block (0x8F0D61-0x8F0E86) -> patch to jmp; part of beat/measure renderer 0x8F0CA0
    .BPM_BAR_PATCH            = nullptr, // bpm gradient draw tail (0x8F0E83-0x8F0E87 = mov rcx,rdi; call rbx) exists but slots unresolved; leave null
    .BPM_BAR_PATCH_JMP        = 0,
    .PLAY_FIELD_LOAD          = base + 0x00808cd0, // play field note table fill zone 0x8085xx-0x808Dxx (add-note core 0x8087E0, note pool ctor 0x807FA0); main entry candidate
    .RETRY_CHECK_A            = nullptr, // no EFFECT/VEFX pair in button-helper callers (helpers 0x80B160/0x80B200); retry may read buttons inline in fail-revive logic 0x8FFD50
    .RETRY_CHECK_B            = nullptr,
    .GHOST_TARGET_FN          = nullptr, // in-play target text drawn via digit sprites (0x90A030 target sabun), not ASCII renderer
    .GRAPH_TARGET_FN          = nullptr,
    .GRAPH_CONDITION          = nullptr,
    .FAIL_ANIMATION_FN        = nullptr, // fail flow restructured: trigger sub_18095CEE0 (gauge zero), dead write sub_1808E9400; anim/sfx/state split across gauge + draw chain
    .FAIL_PLAY_SFX_FN         = nullptr,
    .FAIL_DURATION_JMP        = nullptr,

    .WNDPROC_FN               = base + 0x009253e0, // PropClassWindowProc (renderer hook kept off for smoke)

    .TIMING_HOOK_FN           = nullptr, // 34 has no pre-play set_timing dispatcher; 0x80D/0x80E confirmed no jump-table dispenser
    .ATTRACT_SELECT_FN        = base + 0x008b1610, // call 0x95E380 pick next attract demo chart (byte-identical call site to ref)

    .GET_PLAY_OPTIONS_FN      = base + 0x00871910, // mov rax, cs:qword_18b3e9f28; retn
    .SPRITE_DRAW_FN           = base + 0x00334ed0, // creates a named sprite at its native position and layer
    .JUDGE_APPLY_FN           = base + 0x0080d480, // applies a note judgment; used to obtain ms timing info
    .JUDGE_DISPLAY_FN         = base + 0x008f4bf0, // updates judge, combo, fast/slow to be drawn
    .JUDGE_DRAW_FS_KEYS_FN    = base + 0x008f43c0, // draws FAST/SLOW indicator for keys (or both if combined)
    .JUDGE_DRAW_FS_SC_FN      = base + 0x008f46b0, // draws the separate scratch FAST/SLOW indicator
    .JUDGE_DISPLAY_INIT_FN    = base + 0x008f4980, // initialises one player's judgment display state
    .JUDGE_PRESS_RETURN       = base + 0x0080e0b1, // CONFIRMED by runtime capture: judge_apply caller (grades incl PGREAT)
    .JUDGE_RELEASE_RETURN     = base + 0x0080d05a, // CONFIRMED by runtime capture: judge_apply caller (CN/hold release path)

    // offsets: data
    .GAME_MODEL               = base + 0x01041fd0, // "***:*:*:*:**********" mutable copy (rev writer sub_180958680 stores to +8)
    .GAME_STATE               = base + 0x0b60b3a0, // state_t blob: setters/getters confirmed (game_type@+0 sub_18092FEE0, play_style@+4 sub_18092FEC0, diff@+8/+C sub_18092FD10, active@+10/+14 sub_18092FCC0, active_music@+30 sub_18092FC90)
    .INPUT_MANAGER            = base + 0x00bce280, // InputManagerIIDX singleton: vftable ptr 0x180D9FF20 written to 0x18BCCE280 (line 3372829); data at +8
    .PLAYER_BLOCK             = base + 0x0b1281c0, // play engine global (getter sub_180810FF0: lea rax,[0B1281C0];ret); AUTO_PLAY = +0x2A29FC; per-player stride 0x101560
    .SCORES_P1                = nullptr, // 34 has no large static score tables (0x1858C60 constant absent); scores are per-lobby/league heap - unresolved
    .SCORES_P2                = nullptr,
    .RIVAL_SCORES_P1          = nullptr,
    .RIVAL_SCORES_P2          = nullptr,
    .MAX_ENTRIES              =        0x00080e8, // music entries per play style in the score tables
    .D3D9_DEVICE              = base + 0x0b60bfe0, // [candidate] pointer to the game's IDirect3DDevice9
    .AUTO_PLAY                = base + 0x0b3cabbc, // play engine (getter sub_180810FF0: lea rax,[0B1281C0]; ret) + 0x2A29FC; cleared at song start
    .RANDOM_DATA              = nullptr, // no 2x8 static column table; shuffle is per-start seeded struct 0x0B125990 (hook 0x80F6B0 to force identity/seed 0)
    .PLAY_STATE               = nullptr, // no 0x368-stride play_state_t; live counters on playfield static (EX +0x240, judged +0x238, frame +0x234, note mirror 0x0B3E9FB8+8+4*p)
    .PLAY_SESSION             = base + 0x0b4dda70, // session block (getter sub_1808F02B0); in_gameplay at +0xC8 (NOT +0x54); pacemaker type per-player in options array +0x250+0xBC*p
    .DEAD_STATE               = base + 0x0b4dd620  // per-player alive flags (getter sub_1808E94F0; byte +0/+1 = p1/p2 dead, reset sub_1808E9090)
});