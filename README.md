# Building and Installing Geany Terminal Plugin

# Compile the plugin
gcc -shared -o terminal_plugin.so terminal_plugin.c -fPIC `pkg-config --cflags geany gtk+-3.0 vte-2.91` `pkg-config --libs geany gtk+-3.0 vte-2.91`

# Copy the compiled plugin to Geany's plugin directory
cp terminal_plugin.so ~/.config/geany/plugins/

# Restart Geany and enable the plugin in Tools > Plugin Manager
