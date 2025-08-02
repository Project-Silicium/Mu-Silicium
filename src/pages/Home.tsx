import { Link } from 'react-router-dom';
import { HelpCircle, Zap, ArrowDown, Github, Cpu, Smartphone, Shield, ExternalLink, MessageCircle } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { Badge } from '@/components/ui/badge';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/components/ui/card';
import { TooltipTerm } from '@/components/ui/tooltip-term';

const Home = () => {
  const scrollToFeatures = () => {
    document.getElementById('features-section')?.scrollIntoView({ 
      behavior: 'smooth' 
    });
  };

  const stats = [
    { number: "40+", label: "Supported Devices" },
    { number: "239", label: "GitHub Stars" },
    { number: "103", label: "Forks" },
    { number: "50+", label: "Contributors" }
  ];

  return (
    <div className="min-h-full">
      {/* Hero Section */}
      <section className="relative min-h-screen bg-gradient-hero flex items-center justify-center overflow-hidden">
        {/* Background Pattern */}
        <div className="absolute inset-0 opacity-10">
          <div className="absolute inset-0 bg-gradient-to-br from-primary/20 to-accent/20"></div>
          <div className="absolute top-1/4 left-1/4 w-64 h-64 bg-primary/10 rounded-full blur-3xl animate-pulse"></div>
          <div className="absolute bottom-1/4 right-1/4 w-96 h-96 bg-accent/10 rounded-full blur-3xl animate-pulse delay-1000"></div>
        </div>

        {/* Full Section Background Image */}
        <div 
          className="absolute inset-0 opacity-20 bg-no-repeat bg-cover"
          style={{
            backgroundImage: 'url(./silicon.png)',
            filter: 'blur(1px)',
            backgroundPosition: 'center 20%',
          }}
        ></div>

        <div className="container mx-auto px-6 relative z-10">
          <div className="text-center max-w-4xl mx-auto">
            {/* Title */}
            <h1 className="text-5xl md:text-7xl font-bold mb-6 leading-tight">
              Welcome to{' '}
              <span className="text-gradient-primary animate-glow">
                Project Silicium
              </span>
            </h1>

            {/* Subtitle */}
            <p className="text-xl md:text-2xl text-muted-foreground mb-8 leading-relaxed">
              Building <TooltipTerm term="UEFI firmware">UEFI firmware</TooltipTerm> for <TooltipTerm term="ARM">ARM-based</TooltipTerm> mobile devices
            </p>

            <p className="text-lg text-muted-foreground mb-12 max-w-2xl mx-auto">
              Running <TooltipTerm term="Windows ARM64">Windows 10/11 ARM</TooltipTerm> and Linux on smartphones and tablets with desktop-class performance
            </p>

            {/* CTA Buttons */}
            <div className="flex flex-col sm:flex-row gap-4 justify-center mb-16">
              <Button 
                variant="hero" 
                size="lg" 
                onClick={scrollToFeatures}
                className="group"
              >
                Explore Features
                <ArrowDown className="ml-2 h-5 w-5 group-hover:animate-bounce" />
              </Button>
              <Button 
                variant="cta" 
                size="lg"
                onClick={() => window.open('https://github.com/Project-Silicium', '_blank')}
              >
                <Github className="mr-2 h-5 w-5" />
                View on GitHub
              </Button>
            </div>

            {/* Stats */}
            <div className="grid grid-cols-2 md:grid-cols-4 gap-8 max-w-2xl mx-auto">
              {stats.map((stat, index) => (
                <div key={index} className="text-center">
                  <div className="text-2xl md:text-3xl font-bold text-primary mb-1">
                    {stat.number}
                  </div>
                  <div className="text-sm text-muted-foreground">
                    {stat.label}
                  </div>
                </div>
              ))}
            </div>
          </div>
        </div>

        {/* Floating Image */}
        <div className="absolute bottom-10 right-10 opacity-20 animate-float hidden lg:block">
          <Smartphone className="w-32 h-32 text-primary" />
        </div>
      </section>

      {/* Features Section */}
      <section id="features-section" className="py-24 bg-background">
        <div className="container mx-auto px-6">
          <div className="text-center mb-16">
            <h2 className="text-3xl md:text-4xl font-bold mb-4">
              Key 
              <span className="text-gradient-primary"> Features</span>
            </h2>
            <p className="text-xl text-muted-foreground max-w-2xl mx-auto">
              Discover what makes Project Silicium revolutionary
            </p>
          </div>

          <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-8">
            <Card className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant group">
              <CardHeader className="text-center">
                <div className="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center mx-auto mb-4 group-hover:bg-primary/20 transition-colors">
                  <Cpu className="h-8 w-8 text-primary" />
                </div>
                <CardTitle className="text-xl group-hover:text-primary transition-colors">
                  Windows On ARM
                </CardTitle>
              </CardHeader>
              <CardContent>
                <CardDescription className="text-center text-muted-foreground leading-relaxed">
                  Boot Windows and Linux directly on ARM phones and tablets with our community-driven UEFI builds and give it a whole new life
                </CardDescription>
              </CardContent>
            </Card>

            <Card className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant group">
              <CardHeader className="text-center">
                <div className="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center mx-auto mb-4 group-hover:bg-primary/20 transition-colors">
                  <Shield className="h-8 w-8 text-primary" />
                </div>
                <CardTitle className="text-xl group-hover:text-primary transition-colors">
                  True UEFI Experience
                </CardTitle>
              </CardHeader>
              <CardContent>
                <CardDescription className="text-center text-muted-foreground leading-relaxed">
                  Desktop-grade <TooltipTerm term="UEFI firmware">UEFI firmware</TooltipTerm> brings <TooltipTerm term="secure boot">enterprise-level security</TooltipTerm> and boot capabilities to mobile devices
                </CardDescription>
              </CardContent>
            </Card>

            <Card className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant group">
              <CardHeader className="text-center">
                <div className="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center mx-auto mb-4 group-hover:bg-primary/20 transition-colors">
                  <Smartphone className="h-8 w-8 text-primary" />
                </div>
                <CardTitle className="text-xl group-hover:text-primary transition-colors">
                  Device Compatibility
                </CardTitle>
              </CardHeader>
              <CardContent>
                <CardDescription className="text-center text-muted-foreground leading-relaxed">
                  Growing support for popular smartphones and tablets with detailed compatibility status and installation guides
                </CardDescription>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Windows on ARM Section */}
      <section className="py-24 bg-card/20">
        <div className="container mx-auto px-6">
          <div className="flex flex-col md:flex-row items-center md:space-x-12 max-w-6xl mx-auto">
            <div className="md:w-1/2 mb-8 md:mb-0">
              <img 
                src="./windows.png" 
                alt="Windows on ARM" 
                className="w-full h-[400px] object-contain rounded-lg shadow-elegant hover:shadow-xl transition-all duration-300" 
              />
            </div>
            <div className="md:w-1/2">
              <h2 className="text-3xl md:text-4xl font-bold mb-6">
                <span className="text-gradient-primary">Windows</span> On ARM
              </h2>
              <p className="text-lg text-muted-foreground leading-relaxed mb-8">
                Boot Windows and Linux directly on <TooltipTerm term="ARM">ARM</TooltipTerm> phones and tablets with our community-driven <TooltipTerm term="UEFI">UEFI</TooltipTerm> builds and give it a whole new life. Experience desktop-class performance on your mobile device.
              </p>
              <div className="flex flex-col sm:flex-row gap-4">
                <Button variant="hero" size="lg" className="group">
                  Learn More
                  <ExternalLink className="ml-2 h-5 w-5 group-hover:translate-x-1 transition-transform" />
                </Button>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* True UEFI Experience Section */}
      <section className="py-24 bg-background">
        <div className="container mx-auto px-6">
          <div className="flex flex-col md:flex-row-reverse items-center md:space-x-reverse md:space-x-12 max-w-6xl mx-auto">
            <div className="md:w-1/2 mb-8 md:mb-0">
              <img 
                src="./UEFI.png" 
                alt="UEFI Experience" 
                className="w-full h-[400px] object-contain rounded-lg shadow-elegant hover:shadow-xl transition-all duration-300" 
              />
            </div>
            <div className="md:w-1/2">
              <h2 className="text-3xl md:text-4xl font-bold mb-6">
                True <span className="text-gradient-primary">UEFI Experience</span> for Mobile Devices
              </h2>
              <p className="text-lg text-muted-foreground leading-relaxed mb-8">
                Project Silicium is an open-source initiative that brings desktop-grade <TooltipTerm term="UEFI">UEFI</TooltipTerm> to smartphones and tablets — unlocking support for Windows, Linux, and beyond with <TooltipTerm term="secure boot">enterprise-level security</TooltipTerm>.
              </p>
              <div className="flex flex-col sm:flex-row gap-4">
                <Button variant="cta" size="lg" className="group">
                  <Shield className="mr-2 h-5 w-5" />
                  Explore Security Features
                </Button>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* Device Support Section */}
      <section className="py-24 bg-card/20">
        <div className="container mx-auto px-6">
          <div className="flex flex-col md:flex-row items-center md:space-x-12 max-w-6xl mx-auto">
            <div className="md:w-1/2 mb-8 md:mb-0">
              <img 
                src="./phones2.jpg" 
                alt="Supported Devices" 
                className="w-full h-[400px] object-contain rounded-lg shadow-elegant hover:shadow-xl transition-all duration-300" 
              />
            </div>
            <div className="md:w-1/2">
              <h2 className="text-3xl md:text-4xl font-bold mb-6">
                Is Your <span className="text-gradient-primary">Device Supported?</span>
              </h2>
              <p className="text-lg text-muted-foreground leading-relaxed mb-8">
                Curious whether your phone or tablet works with our <TooltipTerm term="UEFI">UEFI</TooltipTerm> builds? Check our comprehensive device database for up-to-date compatibility info and status of your device.
              </p>
              <Link to="/devices">
  		<Button variant="hero" size="lg" className="group">
 		   View All Devices
  		  <ExternalLink className="ml-2 h-5 w-5 group-hover:translate-x-1 transition-transform" />
		  </Button>
		</Link>
            </div>
          </div>
        </div>
      </section>

    {/* Device Porting Section */}
      <section className="py-24 bg-card/20">
        <div className="container mx-auto px-6">
          <div className="max-w-4xl mx-auto">
            <Card className="bg-gradient-card border-border/50 hover:border-primary/50 transition-all duration-300 hover:shadow-elegant">
              <CardContent className="p-8 md:p-12 text-center">
                <HelpCircle className="w-16 h-16 mx-auto text-primary mb-6" />
                <h2 className="text-3xl md:text-4xl font-bold mb-4">
                  Not Seeing Your Device?
                </h2>
                <p className="text-xl text-muted-foreground mb-6">
                  Don't worry—if your phone or tablet isn't listed, you can port our UEFI firmware yourself!
                </p>
                <p className="text-lg text-muted-foreground leading-relaxed max-w-3xl mx-auto mb-8">
                  Our detailed Porting Guide walks you step-by-step through building and flashing Silicium onto any <TooltipTerm term="ARM">ARM-based</TooltipTerm> device. 
                  Whether you're a seasoned developer or just getting started, we've got code samples, tooling tips, 
                  and troubleshooting advice to get you up and running.
                </p>
                <Button 
                  variant="hero" 
                  size="lg"
                  onClick={() => window.location.href = '/guides'}
                  className="group"
                >
                  See the Porting Guide
                  <ExternalLink className="ml-2 h-5 w-5 group-hover:translate-x-1 transition-transform" />
                </Button>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Community Support Section */}
      <section className="py-24 bg-background">
        <div className="container mx-auto px-6">
          <div className="flex flex-col items-center text-center max-w-4xl mx-auto">
            <img
              src="./support.png"
              alt="24/7 Support"
              className="w-48 h-48 rounded-lg shadow-elegant hover:shadow-xl transition-all duration-300 mb-8"
            />
            <h2 className="text-3xl md:text-4xl font-bold mb-6">
              Always Here to Help <span className="text-gradient-primary"></span>
            </h2>
            <p className="text-lg text-muted-foreground leading-relaxed max-w-3xl mx-auto mb-8">
              Got questions, issues, or want to contribute? Our community and maintainers are available around the clock to assist and guide you through every step. 
              Whether you're a developer or a first-time user, our team and contributors are here to help you troubleshoot, explore, and succeed.
            </p>
          </div>
        </div>
      </section>

  

      {/* Community Links Section */}
      <section className="py-16 bg-card/20">
        <div className="container mx-auto px-6">
          
          <div className="flex justify-center items-center gap-8 max-w-md mx-auto">
           
            <a 
              href="https://discord.com/invite/Dx2QgMx7Sv" 
              target="_blank" 
              rel="noopener noreferrer"
              className="group"
            >
              <div className="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center group-hover:bg-primary/20 transition-colors">
                <MessageCircle className="w-8 h-8 text-primary" />
              </div>
            </a>

            <a 
              href="https://github.com/Project-Silicium" 
              target="_blank" 
              rel="noopener noreferrer"
              className="group"
            >
              <div className="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center group-hover:bg-primary/20 transition-colors">
                <Github className="w-8 h-8 text-primary" />
              </div>
            </a>
          </div>
        </div>
      </section>
    </div>
  );
};

export default Home;
