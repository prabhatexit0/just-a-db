{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    llvm_17
    bazel
    libcxxabi
  ];
}
