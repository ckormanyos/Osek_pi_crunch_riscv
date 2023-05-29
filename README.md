Osek_pi_crunch_riscv
==================

<p align="center">
    <a href="https://github.com/ckormanyos/Osek_pi_crunch_riscv/actions">
        <img src="https://github.com/ckormanyos/Osek_pi_crunch_riscv/actions/workflows/Osek_pi_crunch_riscv.yml/badge.svg" alt="Build Status"></a>
</p>

This repository uses an OSEK-like OS on bare-metal RISC-V FE310 to calculate $\pi$
with a spigot algorithm.

Osek_pi_crunch_cm3 is a fascinating, educational and fun project
that computes a up to $100,001$ decimal digits of $\pi$
on a bare-metal ARM(R) Cortex(R)-M3 system.

The backbone real-time operating system is taken directly
from the OSEK-like OS implemented in
[Chalandi/OSEK](https://github.com/Chalandi/OSEK)

# Software Details

To compute $\pi$, we use a (slow) quadratic pi-spigot algorithm
of order $N^2$ in this project. The spigot calculation
(having quadratic order) is slower than other well-known algorithms
such as AGM or fast series.

The required memory grows linearly with the digit count.
Approximately 1.4 Mbyte RAM are needed for the full $10^{5}$
decimal-digit calculation. Since this is significantly more RAM
than is available on-chip, a slow external serial SPI SRAM is used
for storage.

GNU/GCC `riscv64-unknown-elf` is used for target system
development on `*nix`. The build system is based on
Standard GNUmake/shell-script.

# Prototype Project

This repo features a fully-worked-out prototype example project.
The prototype runs on a RISC-V FE310 controller fitted on the
SparkFun _Red_ _Thing_ RISC-V FE310 Board. The board is driven in OS-less, bare-metal mode.

