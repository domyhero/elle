# -*- encoding: utf-8 -*-

from genshi import template
import threading

from meta import conf

class Viewer(object):
    def __init__(self, loader):
        self.loader = loader
        self.pages = {}

    def render(self, page, obj={}):
        tmpl = self.loader.load(page, encoding=conf.ENCODING)
        tmpl = tmpl.generate(**obj)
        return tmpl.render('html', doctype='html5')

class ViewerStore(threading.local):
    def __init__(self):
        self._viewers = {}

    def get(self, template_dir, auto_reload, encoding):
        viewer = self._viewers.get(template_dir)
        if viewer is None:
            self._viewers[template_dir] = viewer = Viewer(
                template.TemplateLoader(
                    template_dir,
                    auto_reload=auto_reload,
                    default_encoding=encoding
                )
            )
        return viewer
