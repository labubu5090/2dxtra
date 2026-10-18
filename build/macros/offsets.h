#pragma once

#include <optional>
#include <string>
#include <vector>
#include <windows.h>

struct offsets
{
    std::string GAME_VERSION;

    std::uintptr_t DLL_CODE_SIZE;
    std::uintptr_t DLL_ENTRYPOINT;
    std::uintptr_t DLL_IMAGE_SIZE;

    // offsets: code
    std::uint8_t* SOFT_REV_PATCH;         // call replaced by a store of the soft revision
    std::uint8_t* INPUT_POLL_FN;          // IO::InputManagerIIDX vftable slot 3
    std::uint8_t* ARENA_PHASE_PATCH;      // reads the arena phase; stubbed to force local arena
    std::uint8_t* MUSIC_SELECT_CTOR;      // builds the music list; runs once per music select
    std::uint8_t* DAN_SELECT_CTOR;        // CDanSelectScene scene-enter virtual
    std::uint8_t* SCENE_DTOR;             // scene teardown; disables chart switching
    std::uint8_t* STAGE_RESULT_FN;        // CStageResultScene scene-enter virtual
    std::uint8_t* RESET_STATE_FN;         // clears one player's game data on card in
    std::uint8_t* MSELECT_GENRE_C;        // renders the hovered entry's genre and title
    std::uint8_t* MSELECT_GENRE_B;        // `cmp music_entry::texture_genre, 0` inside the above
    std::uint8_t* MSELECT_GENRE_A;        // call inside the above that draws the genre text
    std::uint8_t* MSELECT_GENRE_TEXT_FN;  // the wide/UTF-16 text renderer it calls
    std::uintptr_t MSELECT_GENRE_ENTRY_OFFSET; // the select-data -> music entry pointer displacement
    std::uint8_t* INIT_TEXT_RENDER_FN;    // boot debug text renderer ("Initialize Game")
    std::uint8_t* BOOT_MODEL_TEXT_CALL;   // call that draws the boot model string
    std::uint8_t* TITLE_MODEL_TEXT_CALL;  // call that draws the title-screen model string
    std::uint8_t* TEXT_INIT_FN;           // initialises a text property block
    std::uint8_t* TEXT_RENDER_FN;         // ASCII text renderer
    std::uint8_t* RESULT_ARTIST_FN;       // call that draws the artist line on the result screen
    std::uint8_t* LOAD_CHART_FN_B;        // reads one .1 chart into the scratch buffer
    std::uint8_t* LOAD_CHART_FN_A;        // outer loader; knows which player is loading
    std::uint8_t* REMAP_INDEX_FN;         // in-game difficulty index -> .1 chart index
    std::uint8_t* CHART_ANALYZE_FN;       // chart load + analyze pass run during boot
    std::uint8_t* CHART_ANALYZE_RESULT;   // instruction after the chart read (rax = bytes, rdi = buffer)
    std::uint8_t* LOAD_CHART_READ_RESULT; // instruction after the play loader's final chart read (rax = bytes)
    std::uint8_t* CHART_CALC_RADAR_FN;    // computes the six radar values of a difficulty
    std::uint8_t* GET_APP_CONFIG;         // returns CApplicationConfig
    std::uint8_t* GET_MUSIC_DATA;         // returns the music_data.bin blob
    std::uint8_t* LOAD_AUDIO_FN;          // loads a sound file into the sound bank
    std::uint8_t* GET_SOUND_ENTRY_FN;     // returns the sound bank entry for a sound index
    std::uint8_t* XRPC_APPLY_FN;          // the game's wrapper around avs2-ea3 xrpc_apply
    std::uint8_t* REG_DISPATCH_FN;        // builds and fires the score save request
    std::uint8_t* REG_PATCH_ADDR;         // the call inside it that queues the request
    std::uint8_t* SCORE_INVALID_FN;       // returns whether this play may save
    std::uint8_t* DAN_SAVE_FN;            // queues the dan course result
    std::uint8_t* EAAPPLI_SAVE_FN;        // queues the eamuse app result
    std::uint8_t* MDATA_LOAD_FN;          // runs shortly after music_data is populated
    std::uint8_t* IS_BTN_DOWN_FN_A;       // autoplay gate in the "button pressed" helper
    std::uint8_t* IS_BTN_DOWN_FN_B;       // autoplay gate in the "button held" helper
    std::uint8_t* AUTO_BEAM_PATCH;        // autoplay gate in the lane beam renderer
    std::uint8_t* AUTO_BEAM_FN;           // that renderer; rcx -> block whose first dword is the player
    std::uint8_t* CARD_OUT_VFUNC;         // slot 0 of the CCardOutScene vftable
    std::uint8_t* RENDERER_PATCH;         // call nop'd while the swap chain vtable is swapped
    std::uint8_t* APPLY_RANDOM_FN;        // applies the random modifier
    std::uint8_t* DARK_MODE_PATCH;        // branch over the play field draw
    std::uint8_t* MEASURE_PATCH;          // branch over the measure bar draw
    std::uint8_t* BPM_BAR_PATCH;          // start of the pulsing bpm bar draw
    std::uintptr_t BPM_BAR_PATCH_JMP;     // relative jump past that draw
    std::uint8_t* PLAY_FIELD_LOAD;        // fills the play field note table
    std::uint8_t* RETRY_CHECK_A;          // quick retry check that tests the EFFECT bit
    std::uint8_t* RETRY_CHECK_B;          // quick retry check that tests the VEFX bit
    std::uint8_t* GHOST_TARGET_FN;        // call that draws the "+????" pacemaker ghost text
    std::uint8_t* GRAPH_TARGET_FN;        // call that draws the "TARGET:+????" pacemaker text
    std::uint8_t* GRAPH_CONDITION;        // gate on whether that text is drawn at all
    std::uint8_t* FAIL_ANIMATION_FN;      // call that starts the stage failed animation
    std::uint8_t* FAIL_PLAY_SFX_FN;       // call that plays the stage failed sound effect
    std::uint8_t* FAIL_DURATION_JMP;      // branch checking whether that animation is done
    std::uint8_t* WNDPROC_FN;             // "PropClassWindowProc" for the game window
    std::uint8_t* TIMING_HOOK_FN;         // last chance to change the judge windows before play
    std::uint8_t* ATTRACT_SELECT_FN;      // picks the next attract demo chart
    std::uint8_t* GET_PLAY_OPTIONS_FN;    // returns player options
    std::uint8_t* SPRITE_DRAW_FN;         // creates a named sprite at its native position and layer
    std::uint8_t* JUDGE_APPLY_FN;         // applies a note judgment; used to obtain ms timing info
    std::uint8_t* JUDGE_DISPLAY_FN;       // updates judge, combo, fast/slow to be drawn
    std::uint8_t* JUDGE_DRAW_FS_KEYS_FN;  // draws FAST/SLOW indicator for keys (or both if combined)
    std::uint8_t* JUDGE_DRAW_FS_SC_FN;    // draws the separate scratch FAST/SLOW indicator
    std::uint8_t* JUDGE_DISPLAY_INIT_FN;  // initialises one player's judgment display state
    std::uint8_t* JUDGE_PRESS_RETURN;     // return address of general timing calculation
    std::uint8_t* JUDGE_RELEASE_RETURN;   // return address of CN release timing calculation

    // offsets: variables
    std::uint8_t* GAME_MODEL;             // the mutable ea3 model string ("***:*:*:*:**********")
    std::uint8_t* GAME_STATE;             // bm2dx::state_t
    std::uint8_t* INPUT_MANAGER;          // the IO::InputManagerIIDX singleton
    std::uint8_t* PLAYER_BLOCK;           // base of player 0's game data block
    std::uint8_t* SCORES_P1;              // player 1 score table
    std::uint8_t* SCORES_P2;              // player 2 score table
    std::uint8_t* RIVAL_SCORES_P1;        // player 1 rival score tables
    std::uint8_t* RIVAL_SCORES_P2;        // player 2 rival score tables
    std::uintptr_t MAX_ENTRIES;           // music entries per play style in those tables
    std::uint8_t* D3D9_DEVICE;            // pointer to the game's IDirect3DDevice9
    std::uint8_t* AUTO_PLAY;              // the game's own autoplay flag
    std::uint8_t* RANDOM_DATA;            // bm2dx::random_data_t
    std::uint8_t* PLAY_STATE;             // bm2dx::play_state_t
    std::uint8_t* PLAY_SESSION;           // bm2dx::play_session_t
    std::uint8_t* DEAD_STATE;             // bm2dx::dead_state_t
};

auto inline resolve_offsets(std::uint8_t* base) -> std::optional<offsets>
{
    auto versions = std::vector<offsets> {};

    // populated at build-time from src/2dxtra/offsets/*.cc
    
versions.push_back({
    .GAME_VERSION             = "2025082500-010",

    .DLL_CODE_SIZE            = 0x0c92600,
    .DLL_ENTRYPOINT           = 0x0b1c7bc,
    .DLL_IMAGE_SIZE           = 0xb8b2000,

    .SOFT_REV_PATCH           = base + 0x0967079, // call replaced by `mov byte ptr [rdi+5], rev`
    .INPUT_POLL_FN            = base + 0x0a84840, // IO::InputManagerIIDX vftable slot 3
    .ARENA_PHASE_PATCH        = base + 0x05943d0, // callee of the arena phase clamp
    .MUSIC_SELECT_CTOR        = base + 0x05f4830, // builds the mlist_sel/thumbnail textures
    .DAN_SELECT_CTOR          = base + 0x08d9980, // CDanSelectScene vftable slot 13 (scene enter)
    .SCENE_DTOR               = base + 0x0982760, // prologue pattern [pattern]
    .STAGE_RESULT_FN          = base + 0x0945f90, // CStageResultScene vftable slot 13 (scene enter)
    .RESET_STATE_FN           = base + 0x05a0bc0, // clears one player's game data block
    .MSELECT_GENRE_C          = base + 0x05f36c0, // renders the hovered entry's genre and title
    .MSELECT_GENRE_B          = base + 0x05f3751, // cmp music_entry::texture_genre, 0 (7 bytes, patched to jmp)
    .MSELECT_GENRE_A          = base + 0x05f382d, // call to the wide genre text renderer
    .MSELECT_GENRE_TEXT_FN    = base + 0x05e11c0, // callee of MSELECT_GENRE_A
    .MSELECT_GENRE_ENTRY_OFFSET = 0x10,
    .INIT_TEXT_RENDER_FN      = base + 0x05e0250, // debug text renderer used for the boot progress line
    .BOOT_MODEL_TEXT_CALL     = base + 0x0957e2b, // call that draws the boot model string
    .TITLE_MODEL_TEXT_CALL    = base + 0x094f3ec, // call that draws the title-screen model string
    .TEXT_INIT_FN             = base + 0x0336b90, // initialises the text property block (first callee)
    .TEXT_RENDER_FN           = base + 0x05e05d0, // public wrapper around the ASCII text draw routine
    .RESULT_ARTIST_FN         = base + 0x0939439, // call that draws the result artist text
    .LOAD_CHART_FN_B          = base + 0x0833d50, // reads one .1 chart into the scratch buffer
    .LOAD_CHART_FN_A          = base + 0x0833e70, // sole caller of LOAD_CHART_FN_B
    .REMAP_INDEX_FN           = base + 0x0833ec0, // maps the in-game difficulty index onto the .1 chart index
    .CHART_ANALYZE_FN         = base + 0x0832cc0, // boot-time chart load + analyze pass
    .CHART_ANALYZE_RESULT     = base + 0x0832ea1, // instruction after the chart fread (rax = bytes read, rdi = buffer)
    .CHART_CALC_RADAR_FN      = base + 0x0833700, // prologue pattern [pattern]
    .GET_APP_CONFIG           = base + 0x0954370, // returns CApplicationConfig
    .GET_MUSIC_DATA           = base + 0x096a540, // returns the music_data.bin blob
    .LOAD_AUDIO_FN            = base + 0x0a9a2f0, // prologue pattern [pattern]
    .GET_SOUND_ENTRY_FN       = base + 0x0a99e90, // prologue pattern [pattern]
    .XRPC_APPLY_FN            = base + 0x0aba6e0, // the game's wrapper around avs2-ea3 xrpc_apply
    .REG_DISPATCH_FN          = base + 0x0933710, // builds and fires the music.reg request
    .REG_PATCH_ADDR           = base + 0x093448e, // call that actually queues the score save request
    .SCORE_INVALID_FN         = base + 0x083a660, // shared by the result scene and the score save request
    .DAN_SAVE_FN              = base + 0x05a8870, // prologue pattern [pattern]
    .EAAPPLI_SAVE_FN          = base + 0x05acab0, // prologue pattern [pattern]
    .MDATA_LOAD_FN            = base + 0x094cc70, // CMonitorCheckScene vftable slot 15 (runs after music_data is loaded)
    .IS_BTN_DOWN_FN_A         = base + 0x082b9cd, // cmp autoplay flag inside the `button pressed` helper
    .IS_BTN_DOWN_FN_B         = base + 0x082ba6d, // cmp autoplay flag inside the `button held` helper
    .AUTO_BEAM_PATCH          = base + 0x0924f9b, // cmp autoplay flag in the lane beam renderer
    .AUTO_BEAM_FN             = base + 0x0924d40, // lane beam renderer; rcx -> per-player block whose first dword is the player index
    .CARD_OUT_VFUNC           = base + 0x0d70a30, // CCardOutScene vftable slot 0
    .RENDERER_PATCH           = base + 0x05b8d12, // call nop'd to freeze rendering while the vtable is swapped
    .APPLY_RANDOM_FN          = base + 0x08301c0, // prologue pattern [pattern]
    .DARK_MODE_PATCH          = base + 0x09218ee, // `je` over the play field draw, patched to an unconditional jmp
    .MEASURE_PATCH            = base + 0x0830abe, // branch over the measure bar draw, forced unconditional
    .BPM_BAR_PATCH            = base + 0x0920cb8, // start of the bpm gradient draw, replaced by a jmp past it
    .BPM_BAR_PATCH_JMP        =        0x00000ba, // relative jump to 0x920d77
    .PLAY_FIELD_LOAD          = base + 0x0829520, // fills the play field note table
    .RETRY_CHECK_A            = base + 0x092e0af, // call testing the EFFECT button
    .RETRY_CHECK_B            = base + 0x092e0c1, // call testing the VEFX button
    .GHOST_TARGET_FN          = base + 0x09258a8, // call that renders the '+????' ghost text
    .GRAPH_TARGET_FN          = base + 0x0a4a8b5, // call that renders the 'TARGET:+????' text
    .GRAPH_CONDITION          = base + 0x0a4a86d, // gate on whether the TARGET text is drawn
    .FAIL_ANIMATION_FN        = base + 0x092e8ab, // call that starts the stage failed animation (dl = play it)
    .FAIL_PLAY_SFX_FN         = base + 0x092e8da, // call that plays the stage failed sound effect (rcx = sound id)
    .FAIL_DURATION_JMP        = base + 0x092e270, // `jl` deciding whether the failed animation is still running
    .WNDPROC_FN               = base + 0x0955850, // PropClassWindowProc
    .TIMING_HOOK_FN           = base + 0x082e340, // jump-table dispatcher that installs the judge windows
    .ATTRACT_SELECT_FN        = base + 0x08b1610, // picks the next attract demo chart

    .GET_PLAY_OPTIONS_FN      = base + 0x0897300, // returns player options
    .SPRITE_DRAW_FN           = base + 0x033cf60, // creates a named sprite at its native position and layer
    .JUDGE_APPLY_FN           = base + 0x082dcd0, // applies a note judgment; used to obtain ms timing info
    .JUDGE_DISPLAY_FN         = base + 0x09291d0, // updates judge, combo, fast/slow to be drawn
    .JUDGE_DRAW_FS_KEYS_FN    = base + 0x0928910, // draws FAST/SLOW indicator for keys (or both if combined)
    .JUDGE_DRAW_FS_SC_FN      = base + 0x0928c00, // draws the separate scratch FAST/SLOW indicator
    .JUDGE_DISPLAY_INIT_FN    = base + 0x0928f70, // initialises one player's judgment display state
    .JUDGE_PRESS_RETURN       = base + 0x082e947, // return address of general timing calculation
    .JUDGE_RELEASE_RETURN     = base + 0x082d8aa, // return address of CN release timing calculation

    // offsets: data
    .GAME_MODEL               = base + 0x1076270, // the mutable copy of the ea3 model string
    .GAME_STATE               = base + 0xa9fa900, // state block; p1_active/p2_active at +0x10/+0x14 pin it down
    .INPUT_MANAGER            = base + 0xafbbbb0, // the InputManagerIIDX singleton (its vftable pointer is written here)
    .PLAYER_BLOCK             = base + 0x31b9fe8, // base of player 0's game data block
    .SCORES_P1                = base + 0x31e8f44, // player 0 score table (player block + the score updater's displacement)
    .SCORES_P2                = base + 0x6b3f0ec, // player 1 score table
    .RIVAL_SCORES_P1          = base + 0x3976968, // player 0 rival score table
    .RIVAL_SCORES_P2          = base + 0x72ccb10, // player 1 rival score table
    .MAX_ENTRIES              =        0x00080e8, // music entries per play style in the score tables
    .D3D9_DEVICE              = base + 0xa9fb540, // graphics manager +0xE0 (object at 0xa9fb460; delta assumed) [assumed]
    .AUTO_PLAY                = base + 0xa7b7f0c, // autoplay flag inside the play data block
    .RANDOM_DATA              = base + 0xa512ce0, // object holding the post-random lane order
    .PLAY_STATE               = base + 0xa510bf0, // per-play score/note counters and the pacemaker target
    .PLAY_SESSION             = base + 0xa8ccf10, // gameplay session block (pacemaker type, personal best, in-game flag)
    .DEAD_STATE               = base + 0xa8ccac0 // per-player alive flags
});

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


    auto const dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    auto const nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos->e_lfanew);

    for (auto&& version: versions)
    {
        if (nt->OptionalHeader.SizeOfCode != version.DLL_CODE_SIZE)
            continue;

        if (nt->OptionalHeader.AddressOfEntryPoint != version.DLL_ENTRYPOINT)
            continue;

        if (nt->OptionalHeader.SizeOfImage != version.DLL_IMAGE_SIZE)
            continue;

        return version;
    }

    return std::nullopt;
}
