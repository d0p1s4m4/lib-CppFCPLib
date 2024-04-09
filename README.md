<div align="center">

# libFCP++

</div>

[Hyphanet](https://www.hyphanet.org/) Client Protocol library.

## Usage

```cpp
    fcp::Client client("Demo");

    client.Connect("127.0.0.1", 9481);

    // Do stuff

    client.Disconnect();
```

## License

<img src="https://opensource.org/wp-content/themes/osi/assets/img/osi-badge-light.svg" align="right" height="128px" alt="OSI Approved License">

libFCP++ is licensed under the 3-Clause BSD License.

The full text of the license can be accessed via [this link](https://opensource.org/licenses/BSD-3-Clause) and is also included in [the license file](./LICENSE) of this software package.