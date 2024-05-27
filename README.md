# GleebleGlob

A totally revolutionary and futuristic video streaming command-line utility.

## Links

- Home Page: [https://gleebleglob.club](https://gleebleglob.club)

## Authors

- Saarang Anand <saa183@sfu.ca>
- Micah Baker <mdb15@sfu.ca>
- Arya Faghihy <mfa90@sfu.ca>
- Cohen ter Heide <support@lumpology.com>

## Preface

GleebleGlob is something we came up because it was funny - it doesn't mean anything.

Thanks for that, Saarang.

To describe GleebleGlob, I'm going to use its initialism rather than its expanded form: GG.

The domain however, we will always argue, is the greatest domain known to humanity.

(Good luck remembering it, as we've all gone through mistakes of geebleglop, beegleblog, gooblegleb, ...)

## StormHacks Track: Social / Connectivity

GG fits into the Social / Connectivity track of the [SFU Surge StormHacks 2024 Hackathon](https://stormhacks.com/), which GG was created during/for.

But... what does GG do in this track?

To put it simply, GG enhances your social experience in the terminal, with digital connectivity in the form of convenient video sharing among friends.

GG is like YouTube, but for private videos shared among your friends, and viewed entirely from the terminal, in ASCII.

Yeah, that's right, friggin' ASCII.

## Received Awards

GleebleGlob was lucky to receive the following awards from StormHacks 2024:

- Runner-Up Award (Honourable Mention)
- Best GoDaddy Domain Award
- CSSS Mitchell Gale Award

## Usage

As mentioned, GG is meant to be utilized via the terminal.

Here is the usage information as given by the command `gg help`:

```
gg - v0.2 - [g]leeble[g]lob command line interface.
Check out https://gleebleglob.club for service information.
GENERAL USAGE:
	gg [command] [...arguments]
	NOTE: excess arguments will be ignored.
COMMANDS:
	gg help
	gg version
	gg register [username]
	gg login [username]
	gg logout
	gg status
	gg upload [file path]
	gg stream [file token]
```

It should be noted that GG keeps track of your current session (previously uploaded and streamed videos) in `~/.ggsession` and `~/.gg{user}` files.

To check and see if you are currently logged in, simply run `gg status`.

It should be noted that `[file path]` must refer to a video file, and that `[file token]` will be provided upon successful uploading of a given file.

You can share the file token given by the upload command with your friends on different machines, as GG interacts with a GG API deployed in Saskatchewan to store and stream video data.

*Remember, like YouTube, but on the CLI!*

## Installation

To install GG, you'll need to compile the C++ source code for the client on your machine.

The GG API is currently deployed to `api.gleebleglob.club`, and the client will automatically connect to this API.

Compiling and using the client has been tested on MacOS and Linux - other Unix operating systems should work given the dependencies are properly installed.

You will need to install the following dependencies, either with `homebrew` (MacOS), `apt` (Debian-descendant), or other package manager:

- CMake (the C/C++ compilation toolset)
- FFmpeg (the command-line utility)
- SDL2 (the cross-platform C/C++ windowing library)

Networking is accomplished with BSD Sockets, which is provided by any POSIX-compliant OS (such as MacOS and Linux), so no additional dependencies need be installed for that.

With these dependencies installed properly, clone the source code from GG's GitHub repo, and run the following commands in order:

```
cd GG
cd client
cmake -S . -B build
./build.sh
sudo cp build/gg /usr/local/bin/gg
```

After which, the `gg` command utility will be available from your terminal.

Refer to the **Usage** section for more information on how to use GG as a CLI.

## Example Streams

(Run `gg stream v_0123456789...`.)

- `v_2226203857264260164`
- `v_17041085916556327171`
- `v_10332929141240834880`
- `v_3276635946889121539`
- `v_16760866251225362132`
- `v_17844767469354373499`
- `v_2421141412007620081`
- `v_383091170352221687`
- `v_16288433935527781744`
- `v_4811431923445735995`
- `v_2986378675574602212`
- `v_13497546436462669978`
- `v_11851069939343523766`
- `v_11656665100777775121`
- `v_17857346302094729642`
- `v_12727808860395156209`
- `v_9815832116368560411`
- `v_9137517046553090184`

Or upload your own video with `gg upload [file path]`!

NOTE: identifiers will not appear on this site.

However, anyone with the identifier of the video you uploaded can stream the video with their installation of GG.
