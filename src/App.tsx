import { Toaster } from "@/components/ui/toaster";
import { Toaster as Sonner } from "@/components/ui/sonner";
import { TooltipProvider } from "@/components/ui/tooltip";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { HashRouter, Routes, Route } from "react-router-dom";
import Layout from "./components/Layout";
import Index from "./pages/Index";
import About from "./pages/About";
import NotFound from "./pages/NotFound";
import Devices from "./pages/Devices";
import Guides from "./pages/Guides";
import Contribute from "./pages/contribute";
import SnapdragonPortingGuide from "./pages/SnapdragonPortingGuide";

const queryClient = new QueryClient();

const App = () => (
  <QueryClientProvider client={queryClient}>
    <TooltipProvider>
      <Toaster />
      <Sonner />
      <HashRouter>
        <Layout>
          <Routes>
            <Route path="/" element={<Index />} />
            <Route path="/about" element={<About />} />
            <Route path="/devices" element={<Devices />} />
            <Route path="/guides" element={<Guides />} />
            <Route path="/contribute" element={<Contribute />} />
	<Route path="/guides/snapdragon-porting" element={<SnapdragonPortingGuide />} />
            {/*CATCH-ALL "*" ROUTE-------- this is the default route */}
            <Route path="*" element={<NotFound />} />
          </Routes>
        </Layout>
      </HashRouter>
    </TooltipProvider>
  </QueryClientProvider>
);

export default App;
