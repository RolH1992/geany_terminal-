#include <geany/geanyplugin.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <stddef.h>

GeanyPlugin    *geany_plugin;
GeanyData      *geany_data;

PLUGIN_SET_INFO(
    "Terminal Plugin",
    "Adds a terminal to Geany",
    "1.0",
    "Your Name"
)

static GtkWidget *vte_terminal = NULL;

static gboolean terminal_plugin_init(GeanyPlugin *plugin, gpointer pdata)
{
    geany_plugin = plugin;
    geany_data = plugin->geany_data;

    GtkWidget *message_window_notebook = geany->main_widgets->message_window_notebook;

    if (!GTK_IS_NOTEBOOK(message_window_notebook)) {
        g_print("Could not find Geany's message window notebook\n");
        return FALSE;
    }

    // Check if the terminal already exists
    gint n_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(message_window_notebook));
    for (gint i = 0; i < n_pages; i++) {
        GtkWidget *page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(message_window_notebook), i);
        const gchar *tab_name = gtk_notebook_get_tab_label_text(GTK_NOTEBOOK(message_window_notebook), page);
        if (g_strcmp0(tab_name, "Terminal") == 0) {
            g_print("Terminal tab already exists\n");
            return TRUE;
        }
    }

    // If we reach here, the terminal doesn't exist, so we create a new one
    vte_terminal = vte_terminal_new();

    gint page_num = gtk_notebook_append_page(GTK_NOTEBOOK(message_window_notebook), 
                                             vte_terminal, 
                                             gtk_label_new("Terminal"));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(message_window_notebook), page_num);
    g_print("Terminal tab added to message window notebook at page %d\n", page_num);

    vte_terminal_spawn_async(VTE_TERMINAL(vte_terminal),
                             VTE_PTY_DEFAULT,
                             NULL,
                             (char *[]){"/bin/sh", NULL},
                             NULL,
                             G_SPAWN_SEARCH_PATH,
                             NULL, NULL, NULL,
                             -1,
                             NULL,
                             NULL,
                             NULL);

    gtk_widget_show_all(vte_terminal);
    return TRUE;
}

static void terminal_plugin_cleanup(GeanyPlugin *plugin, gpointer pdata)
{
    if (vte_terminal) {
        gtk_widget_destroy(vte_terminal);
        vte_terminal = NULL;
    }
}

G_MODULE_EXPORT
void geany_load_module(GeanyPlugin *plugin)
{
    plugin->info->name = "Terminal Plugin";
    plugin->info->description = "Adds a terminal to Geany";
    plugin->info->version = "1.0";
    plugin->info->author = "Your Name";

    plugin->funcs->init = terminal_plugin_init;
    plugin->funcs->cleanup = terminal_plugin_cleanup;
    plugin->funcs->configure = NULL;
    plugin->funcs->help = NULL;

    GEANY_PLUGIN_REGISTER(plugin, 226);
}
