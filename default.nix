{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    bazel_7
    llvmPackages_latest.libstdcxxClang
  ];
}
