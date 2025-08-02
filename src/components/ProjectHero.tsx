import { Button } from "@/components/ui/button";
import { Github, ExternalLink, Cpu, Smartphone } from "lucide-react";
import heroImage from "@/assets/hero-mobile-windows.jpg";

const ProjectHero = () => {
  return (
    <section className="relative min-h-screen bg-gradient-hero flex items-center justify-center overflow-hidden">
      {/* Background Pattern */}
      <div className="absolute inset-0 opacity-10">
        <div className="absolute inset-0 bg-gradient-to-br from-primary/20 to-accent/20"></div>
      </div>

      <div className="container mx-auto px-6 relative z-10">
        <div className="grid lg:grid-cols-2 gap-12 items-center">
          {/* Content */}
          <div className="text-center lg:text-left">
            <div className="flex items-center justify-center lg:justify-start gap-2 mb-6">
              <Cpu className="h-8 w-8 text-primary animate-glow" />
              <span className="text-sm font-semibold text-muted-foreground tracking-wider uppercase">
                Project Silicium
              </span>
            </div>
            
            <h1 className="text-4xl lg:text-6xl font-bold mb-6 leading-tight">
              UEFI on
              <span className="bg-gradient-primary bg-clip-text text-transparent animate-glow">
                {" "}Mobile{" "}
              </span>
              Devices
            </h1>
            
            <p className="text-xl text-muted-foreground mb-8 leading-relaxed max-w-2xl">
              Porting Project Mu UEFI firmware to ARM-based mobile devices, enabling 
              Windows 10/11 ARM64 to run on smartphones and tablets. Breaking boundaries 
              between mobile and desktop computing.
            </p>

            <div className="flex flex-col sm:flex-row gap-4 justify-center lg:justify-start">
              <Button 
                variant="hero" 
                size="lg" 
                className="group"
                onClick={() => window.open('https://github.com/Project-Silicium', '_blank')}
              >
                <Github className="mr-2 h-5 w-5 group-hover:animate-spin" />
                View on GitHub
              </Button>
              <Button variant="cta" size="lg">
                <ExternalLink className="mr-2 h-5 w-5" />
                Documentation
              </Button>
            </div>

            {/* Stats */}
            <div className="flex flex-wrap gap-8 justify-center lg:justify-start mt-12 pt-8 border-t border-border">
              <div className="text-center">
                <div className="text-2xl font-bold text-primary">239</div>
                <div className="text-sm text-muted-foreground">GitHub Stars</div>
              </div>
              <div className="text-center">
                <div className="text-2xl font-bold text-primary">103</div>
                <div className="text-sm text-muted-foreground">Forks</div>
              </div>
              <div className="text-center">
                <div className="text-2xl font-bold text-primary">20+</div>
                <div className="text-sm text-muted-foreground">Supported Devices</div>
              </div>
            </div>
          </div>

          {/* Hero Image */}
          <div className="relative lg:order-last">
            <div className="relative group">
              <div className="absolute -inset-4 bg-gradient-primary rounded-2xl blur-2xl opacity-30 group-hover:opacity-50 transition-all duration-500"></div>
              <img 
                src={heroImage} 
                alt="Mobile device running Windows 11 through Project Silicium UEFI"
                className="relative w-full max-w-lg mx-auto rounded-2xl shadow-elegant border border-border/50 animate-float"
              />
              <div className="absolute top-4 right-4 bg-card/90 backdrop-blur-sm rounded-lg px-3 py-1 border border-border">
                <div className="flex items-center gap-2">
                  <Smartphone className="h-4 w-4 text-primary" />
                  <span className="text-sm font-medium">UEFI Boot</span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
};

export default ProjectHero;
