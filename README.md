# simpleFTPServer

This is a FTP server.
It was code in Qt5.7. However it doesn't have the GUI interface.
Every one is welcome to get this code.

### Use mathod
The file "setting.json" is the configuration file, which was look like this
```json
{
    "PORT": 8989,
    "DIR": "E:/tempftp",
    "THREAD": 16,
    "USER": [{
        "NAME": "123",
        "PASS": "123"
    }, {
        "NAME": "abc",
        "PASS": "abc"
    }]
}
```

- The "PORT" is the port that you want to listen.
- "DIR" is the absolute path of your own file system.
- "THREAD" means how many threads that you want to run in this server. It means that clients can access this server at the same time.
- "USER" is the user who you allow to visit your FTP server.

> One Important thing is that the Json library is came from [https://github.com/nlohmann/json] https://github.com/nlohmann/json
