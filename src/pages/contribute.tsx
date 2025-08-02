import { Github, Users, BookOpen } from "lucide-react";
import { Link } from "react-router-dom";

export default function CommunityHub() {
  return (
    <div className="max-w-5xl mx-auto py-16 px-6 text-center">
      <h1 className="text-4xl md:text-5xl font-bold text-primary mb-4">
        Welcome to the Community Hub
      </h1>
      <p className="text-lg text-muted-foreground max-w-2xl mx-auto mb-10">
        Whether you're a developer, tester, or just a fan of open-source, this is your home. Connect, contribute, and help grow the Project Silicium ecosystem.
      </p>

      <div className="grid grid-cols-1 md:grid-cols-3 gap-6 text-left">
        {/* GitHub Card */}
        <div className="bg-zinc-900 border border-zinc-700 rounded-xl p-6 shadow-lg hover:shadow-xl transition-all">
          <Github className="w-8 h-8 text-white mb-3" />
          <h3 className="text-xl font-semibold text-white mb-2">GitHub</h3>
          <p className="text-zinc-400 text-sm mb-4">
            Browse our code, report issues, or contribute directly to the firmware, bootloaders, and tooling.
          </p>
          <a
            href="https://github.com/Project-Silicium"
            target="_blank"
            className="text-blue-400 hover:underline text-sm font-medium"
          >
            Visit GitHub →
          </a>
        </div>

        {/* Documentation */}
        <div className="bg-zinc-900 border border-zinc-700 rounded-xl p-6 shadow-lg hover:shadow-xl transition-all">
          <BookOpen className="w-8 h-8 text-white mb-3" />
          <h3 className="text-xl font-semibold text-white mb-2">Documentation</h3>
          <p className="text-zinc-400 text-sm mb-4">
            Learn how to build and flash UEFI on your device, debug issues, and understand the internals of the project.
          </p>
          <Link
            to="/docs"
            className="text-blue-400 hover:underline text-sm font-medium"
          >
            Read the Docs →
          </Link>
        </div>

        {/* Community & Chat */}
        <div className="bg-zinc-900 border border-zinc-700 rounded-xl p-6 shadow-lg hover:shadow-xl transition-all">
          <Users className="w-8 h-8 text-white mb-3" />
          <h3 className="text-xl font-semibold text-white mb-2">Join the Community</h3>
          <p className="text-zinc-400 text-sm mb-4">
            Chat with devs, ask questions, share ports, and get real-time support from fellow Silicium fans.
          </p>
          <a
            href="https://discord.gg/Dx2QgMx7Sv"
            target="_blank"
            className="text-blue-400 hover:underline text-sm font-medium"
          >
            Join Discord →
          </a>
        </div>
      </div>
    </div>
  );
}

