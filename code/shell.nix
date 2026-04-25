{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
    wayland-scanner
  ];

  buildInputs = with pkgs; [
    wayland
    wayland-protocols
    libxkbcommon
    libdecor
    libGL

    # X11 fallback — optional but recommended
    libx11
    libxext
    libxcursor
    libxi
    libxrandr
    libxfixes
    libxtst
    libxcb
    libx11.dev
    vulkan-headers
    vulkan-loader
    xorg.libXScrnSaver
    libxxf86vm
    xorgproto
    udev
    gcc-arm-embedded
  ];

  # So the resulting binary can find these libs at runtime too,
  # since SDL dlopens them rather than linking them.
  LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (
    with pkgs;
    [
      wayland
      wayland-protocols
      libxkbcommon
      libdecor
      libGL

      # X11 fallback — optional but recommended
      libx11
      libxext
      libxcursor
      libxi
      libxrandr
      libxfixes
      libxtst
      libxcb
      libx11.dev
      vulkan-headers
      vulkan-loader
      xorg.libXScrnSaver
      libxxf86vm
      xorgproto
      udev
      gcc-arm-embedded
    ]
  );
}
