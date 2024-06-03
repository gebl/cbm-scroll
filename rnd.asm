.global _rnd_init
.global _rnd

_rnd_init:
    LDA #$FF  ; maximum frequency value
    STA $D40E ; voice 3 frequency low byte
    STA $D40F ; voice 3 frequency high byte
    LDA #$80  ; noise waveform, gate bit off
    STA $D412 ; voice 3 control register
    RTS

.global rnd
_rnd:
    LDA $D41B
    RTS