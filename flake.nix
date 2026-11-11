{
  description = "ProcessamentoGraficoTGB";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        qt = pkgs.qt6;

        qtLibs = [
          qt.qtbase
          qt.qtwayland
          qt.qtsvg
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            ninja
            pkg-config
            clang-tools
            gdb
            valgrind
            opencv
          ];

          buildInputs = qtLibs ++ [
            pkgs.libGL
          ];

          CMAKE_EXPORT_COMPILE_COMMANDS = "1";
          CMAKE_GENERATOR = "Ninja";

          QT_PLUGIN_PATH = pkgs.lib.makeSearchPath qt.qtbase.qtPluginPrefix qtLibs;

          shellHook = ''
            echo "C++ / Qt ${qt.qtbase.version} pronto."
            echo "  cmake -B build && cmake --build build"
          '';
        };
      });
}
