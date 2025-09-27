import React, { useState, useEffect } from "react";

// Simple social feed component
// - Post messages (text)
// - See posts immediately in feed (newest on top)
// - Persist to localStorage so posts remain on reload
// - Like and delete actions

export default function SimpleSocialFeed() {
  const [posts, setPosts] = useState([]);
  const [text, setText] = useState("");

  // load from localStorage once
  useEffect(() => {
    const raw = localStorage.getItem("simple_social_posts");
    if (raw) {
      try {
        setPosts(JSON.parse(raw));
      } catch (e) {
        console.error("Failed to parse posts from storage", e);
      }
    }
  }, []);

  // save whenever posts change
  useEffect(() => {
    localStorage.setItem("simple_social_posts", JSON.stringify(posts));
  }, [posts]);

  function handlePost() {
    const trimmed = text.trim();
    if (!trimmed) return;
    const newPost = {
      id: Date.now(),
      text: trimmed,
      createdAt: new Date().toISOString(),
      likes: 0,
    };
    // newest first
    setPosts((p) => [newPost, ...p]);
    setText("");
  }

  function handleLike(id) {
    setPosts((p) => p.map((t) => (t.id === id ? { ...t, likes: t.likes + 1 } : t)));
  }

  function handleDelete(id) {
    setPosts((p) => p.filter((t) => t.id !== id));
  }

  return (
    <div className="min-h-screen bg-slate-50 flex items-start justify-center p-6">
      <div className="w-full max-w-2xl">
        <header className="mb-6">
          <h1 className="text-3xl font-extrabold">เว็บสเต็ก ง่ายๆ</h1>
          <p className="text-sm text-slate-600">โพสต์ข้อความของคุณแล้วจะเห็นทันทีในฟีด</p>
        </header>

        <section className="mb-6 bg-white p-4 rounded-2xl shadow-sm">
          <textarea
            value={text}
            onChange={(e) => setText(e.target.value)}
            rows={4}
            placeholder="เขียนอะไรสักอย่าง..."
            className="w-full resize-none p-3 rounded-lg border border-slate-200 focus:outline-none focus:ring-2 focus:ring-slate-300"
          />
          <div className="mt-3 flex items-center justify-between">
            <div className="text-sm text-slate-500">พร้อมโพสต์</div>
            <div>
              <button
                onClick={handlePost}
                className="px-4 py-2 bg-slate-900 text-white rounded-xl shadow hover:opacity-90 disabled:opacity-50"
                disabled={!text.trim()}
              >
                โพสต์
              </button>
            </div>
          </div>
        </section>

        <section>
          <h2 className="text-xl font-semibold mb-3">ฟีด</h2>
          {posts.length === 0 ? (
            <div className="text-slate-500">ยังไม่มีโพสต์ ลองโพสต์ข้อความแรกดู</div>
          ) : (
            <div className="space-y-4">
              {posts.map((p) => (
                <article key={p.id} className="bg-white p-4 rounded-2xl shadow-sm">
                  <div className="flex items-start justify-between">
                    <div>
                      <div className="font-medium">ผู้ใช้งาน</div>
                      <div className="text-xs text-slate-500">{new Date(p.createdAt).toLocaleString()}</div>
                    </div>
                    <div className="flex items-center gap-2">
                      <button
                        onClick={() => handleLike(p.id)}
                        className="text-sm px-3 py-1 rounded-lg border"
                      >
                        👍 {p.likes}
                      </button>
                      <button
                        onClick={() => handleDelete(p.id)}
                        className="text-sm px-3 py-1 rounded-lg border text-red-600"
                      >
                        ลบ
                      </button>
                    </div>
                  </div>

                  <p className="mt-3 whitespace-pre-line">{p.text}</p>
                </article>
              ))}
            </div>
          )}
        </section>

        <footer className="mt-8 text-xs text-slate-400 text-center">เก็บข้อมูลไว้ใน browser (localStorage)</footer>
      </div>
    </div>
  );
}
