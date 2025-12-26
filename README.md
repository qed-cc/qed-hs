# QED-HS: Hidden Service Network Layer

**Anonymous P2P network with embedded hidden services for the Code Chain**

QED-HS is a privacy-focused network layer that turns every node into a v3 hidden
service. Based on onion routing technology, it provides censorship-resistant
communication for the QED Code Chain.

## Key Features

- **Every node is a hidden service** - No external ports needed
- **Embedded dynhost** - Host services directly in the binary
- **v3 addresses** - Modern cryptographic addressing
- **Self-contained** - Single binary, no external dependencies

## Quick Start

### Build from source:

```bash
git clone git@github.com:qed-cc/qed-hs.git
cd qed-hs
./autogen.sh
./configure
make
```

### Run:

```bash
./src/app/qed-hs/qed-hs
```

The node will automatically start and create an ephemeral hidden service address.
Look for this line in the output:

```
[notice] Dynamic onion host ephemeral service created with address: [address].onion
```

### Connect to a node:

```bash
curl --socks5-hostname 127.0.0.1:9050 http://[address].onion/
```

## Integration with Code Chain

QED-HS provides the P2P layer for the Code Chain:

```
┌─────────────────────────────────────────────────────────┐
│                   Code Chain Node                       │
├─────────────────────────────────────────────────────────┤
│  Application Layer (qedc)                               │
│    - Vision management                                  │
│    - Solution mining                                    │
│    - PoW voting                                         │
├─────────────────────────────────────────────────────────┤
│  Storage Layer (qed-vc)                                 │
│    - Content-addressed blobs (SHA3-256)                 │
│    - Merkle trees for state                             │
├─────────────────────────────────────────────────────────┤
│  P2P Layer (qed-hs)                   ◀── YOU ARE HERE  │
│    - Hidden service for incoming                        │
│    - Onion routing for outgoing                         │
│    - DHT over hidden services                           │
└─────────────────────────────────────────────────────────┘
```

## API

```c
#include <qed_hs.h>

// Initialize the network layer
qed_hs_init();

// Start hidden service
qed_hs_address_t my_address;
qed_hs_start_service(&my_address);

// Connect to peer
qed_hs_connection_t *conn = qed_hs_connect(peer_address);

// Send/receive messages
qed_hs_send(conn, data, len);
qed_hs_recv(conn, buffer, &len);

// Cleanup
qed_hs_shutdown();
```

## Dynhost Feature

The dynhost feature enables hosting services directly within the binary:

- **No external web server** - All service logic is embedded
- **No open ports** - Accessible only via hidden service address
- **MVC Framework** - Rails-like framework for building apps

Available demo endpoints:
- `/` - Main menu
- `/time` - Time server
- `/calculator` - Calculator demo
- `/blog` - MVC blog application

## License

Dual licensed:
- **Apache License 2.0** - All new QED-HS code and dynhost feature
- **BSD 3-Clause** - Original onion routing code

Copyright 2025 Rhett Creighton - Apache License 2.0

## Related Projects

- [qedc](https://github.com/qed-cc/qedc) - Code Chain compiler
- [qed-vc](https://github.com/qed-cc/qed-vc) - Version control with SHA3-256
