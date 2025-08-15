import { SidebarProvider, SidebarTrigger } from "@/components/ui/sidebar";
import { AppSidebar } from "@/components/AppSidebar";
import ProjectFooter from "@/components/ProjectFooter";

interface LayoutProps {
  children: React.ReactNode;
}

export default function Layout({ children }: LayoutProps) {
  return (
    <SidebarProvider>
      <div className="min-h-screen flex w-full bg-background">
        <AppSidebar />
        
        <div className="flex-1 flex flex-col">
          {/* Header with sidebar trigger */}
          <header className="h-14 flex items-center border-b border-border bg-card/50 backdrop-blur-sm">
            <SidebarTrigger className="ml-4" />
            <div className="ml-4">
              <h1 className="text-lg font-semibold">Project Silicium</h1>
            </div>
          </header>

          {/* Main content */}
          <main className="flex-1 overflow-auto">
            {children}
          </main>

          {/* Footer */}
          <ProjectFooter />
        </div>
      </div>
    </SidebarProvider>
  );
}
