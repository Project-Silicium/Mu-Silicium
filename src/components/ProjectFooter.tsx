import { Button } from "@/components/ui/button";
import { Github, ExternalLink, BookOpen, MessageCircle } from "lucide-react";

const ProjectFooter = () => {
  return (
    <footer className="bg-card/30 backdrop-blur-sm border-t border-border">
      <div className="container mx-auto px-6 py-16">
        <div className="grid md:grid-cols-2 lg:grid-cols-4 gap-8">
          {/* Project Info */}
          <div className="lg:col-span-2">
            <h3 className="text-xl font-bold mb-4">Project Silicium</h3>
            <p className="text-muted-foreground mb-6 leading-relaxed">
              Pioneering the future of mobile computing by bringing UEFI firmware 
              to ARM-based mobile devices, enabling full Windows compatibility 
              on smartphones and tablets.
            </p>
            <div className="flex gap-4">
              <Button 
                variant="outline" 
                size="sm"
                onClick={() => window.open('https://github.com/Project-Silicium', '_blank')}
              >
                <Github className="mr-2 h-4 w-4" />
                GitHub
              </Button>
              <Button 
                variant="outline" 
                size="sm"
                onClick={() => window.open('https://github.com/Project-Silicium/Guides', '_blank')}
              >
                <BookOpen className="mr-2 h-4 w-4" />
                Guides
              </Button>
            </div>
          </div>

          {/* Quick Links */}
          <div>
            <h4 className="font-semibold mb-4">Resources</h4>
            <ul className="space-y-2 text-sm">
              <li>
                <a 
                  href="https://github.com/Project-Silicium/Mu-Silicium" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  Mu-Silicium Repository
                </a>
              </li>
              <li>
                <a 
                  href="https://github.com/Project-Silicium/Guides" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  Installation Guides
                </a>
              </li>
              <li>
                <a 
                  href="https://github.com/Project-Silicium/edk2-porting-guide" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  Porting Guide
                </a>
              </li>
              <li>
                <a 
                  href="https://microsoft.github.io/mu/" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  Project Mu
                </a>
              </li>
            </ul>
          </div>

          {/* Community */}
          <div>
            <h4 className="font-semibold mb-4">Community</h4>
            <ul className="space-y-2 text-sm">
              <li>
                <a 
                  href="https://github.com/Project-Silicium/Mu-Silicium/issues" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  Report Issues
                </a>
              </li>
              <li>
                <a 
                  href="https://github.com/Project-Silicium/Mu-Silicium/discussions" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  Discussions
                </a>
              </li>
              <li>
                <a 
                  href="https://github.com/Project-Silicium" 
                  className="text-muted-foreground hover:text-primary transition-colors"
                  target="_blank"
                  rel="noopener noreferrer"
                >
                  All Repositories
                </a>
              </li>
            </ul>
          </div>
        </div>

        <div className="mt-12 pt-8 border-t border-border text-center text-sm text-muted-foreground">
          <p>
            © {new Date().getFullYear()} Project Silicium. Open source project licensed under various licenses. 
            See individual repositories for details.
          </p>
        </div>
        <div className=" text-center text-sm text-muted-foreground">
          <p>
             Made by{" "}
  <a
    href="https://github.com/creepreee"
    target="_blank"
    rel="noopener noreferrer"
    className="text-blue-500 hover:underline"
  >
    @Creepreee
  </a>
          </p>
        </div>
      </div>
    </footer>
  );
};

export default ProjectFooter;
