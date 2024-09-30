CC = gcc
CFLAGS = -fPIC `pkg-config --cflags geany gtk+-3.0 vte-2.91`
LIBS = `pkg-config --libs geany gtk+-3.0 vte-2.91`
PLUGIN = terminal_plugin.so

$(PLUGIN): terminal_plugin.c
	$(CC) -shared -o $(PLUGIN) terminal_plugin.c $(CFLAGS) $(LIBS)

clean:
	rm -f $(PLUGIN)
