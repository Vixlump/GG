# GG Client

## Development Requirements

- `cmake` to compile the source code.
- `pkg-config` to automate the compiler flags.
- `ffmpeg` and its respective development libraries (most importantly `libavutil`).

To set up client development, simply run `cmake -S . -B build` from within the `client` folder.

Thenceforth, run `./build.sh` to build the client software.

## Command Braindump

- `gg register <username> <registration-token>`
  - username: local username for this user
  - will prompt user for a password (this will be stored locally)
  - password is used to encrypt or hide the api key which will be used 
  - will generate a locked `~/.gg<username>` file
- `gg login <options> <username>`
  - options:
    - password: -p/--password password (string)
  - e.g.: `gg login micahdbak`
  - will attempt to unlock the `~/.gg<username>` file and store the decrypted api key in `~/.ggsession`
  - will prompt user for password if not provided
- `gg logout`
  - e.g.: `gg logout`
  - will delete the `~/.ggsession` file (this is not perfectly secure but stfu its fine)
  - will store any new uploads and their passwords in the `~/.gg<username>` file
- `gg upload <options> <file>`
  - options:
    - upload from specific timestamps: -r/--range start:end (seconds:seconds) either when left blank assumes 0:end
    - passkey to encrypt the file: -k/--key key (string)
  - file:
    - some file to be uploaded to the server
  - e.g.: `gg upload -k secret_file ./gleeby.mp4`
  - will get session information from the `~/.ggsession` file and then interact with the gg api
  - will use ffmpeg libraries to "chunk" the given file and encrypt it using the password
  - will prompt user for encryption key if not provided
- `gg stream <options> <id>`
  - options:
    - passkey to decrypt the chunks: -k/--key key (string)
  - will get session information from the `~/.ggsession` file and then interact with the gg api
  - will prompt user for decryption key if not provided

to start:
- `gg login` just creates a .ggsession, and doesn't bother with .gg<username> just yet. (we worry about that later)
- everything is displayed in plaintext; later, we can hide passwords behind '\*' characters
